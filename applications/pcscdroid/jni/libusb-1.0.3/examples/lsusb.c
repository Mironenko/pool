/*
 * libusb example program to list devices on the bus
 * Copyright (C) 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <sys/types.h>

#include <libusb/libusb.h>
#include <stdbool.h>


//int controlUSB(libusb_device_handle *dev_handle, uint16_t interface, uint8_t requesttype, uint8_t request, uint16_t value,
//               unsigned char *bytes, uint16_t size, unsigned int timeout)
//{
//    int ret = libusb_control_transfer(dev_handle, requesttype, request, value, interface,
//                                  bytes, size, timeout);
//    if (ret < 0) {
//        printf("control failed");
//    }
//    return ret;
//}
typedef struct {
    /*
     * Endpoints
     */
    int bulk_in;
    int bulk_out;
    int interrupt;
} Endpoints;

void printBytesHex(unsigned char * buf, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (i > 0)
            printf(" ");
        printf("%02X", buf[i]);
    }
    printf("\n");
}

void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0;

    bool rutokenFound = false;
	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)\n",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

        if(desc.idVendor == 2697) {
            rutokenFound = true;
            printf("\nOpening Rutoken USB device...\n");
            libusb_device_handle *handle = NULL;
            if (libusb_open(dev, &handle) != 0) {
                fprintf(stderr, "unable to open the USB device\n");
            }

            printf("\nGetting active config descriptor...\n");
            struct libusb_config_descriptor *config_desc = NULL;
            if (libusb_get_active_config_descriptor(dev, &config_desc) != 0) {
                fprintf(stderr, "can not get active config descriptor\n");
            }

            printf("\nEnumerating endpoints...\n");
            Endpoints endpoints;
            /* if multiple interfaces use the first one with CCID class type */
            for (i = 0; i < config_desc->bNumInterfaces; i++) {
                /* CCID Class? */
                if (config_desc->interface[i].altsetting->bInterfaceClass == 0xb
                    || config_desc->interface[i].altsetting->bInterfaceClass == 0xff
                        ) {
                    const struct libusb_interface * usb_interface = &config_desc->interface[i];
                    /*
                     * 3 Endpoints maximum: Interrupt In, Bulk In, Bulk Out
                     */
                    for (i = 0; i < usb_interface->altsetting->bNumEndpoints; i++) {
                        /* interrupt end point (if available) */
                        if (usb_interface->altsetting->endpoint[i].bmAttributes
                            == LIBUSB_TRANSFER_TYPE_INTERRUPT) {
                            endpoints.interrupt = usb_interface->altsetting->endpoint[i].bEndpointAddress;
                            continue;
                        }

                        if (usb_interface->altsetting->endpoint[i].bmAttributes
                            != LIBUSB_TRANSFER_TYPE_BULK)
                            continue;

                        uint8_t bEndpointAddress = usb_interface->altsetting->endpoint[i].bEndpointAddress;

                        if ((bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
                            == LIBUSB_ENDPOINT_IN)
                            endpoints.bulk_in = bEndpointAddress;

                        if ((bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
                            == LIBUSB_ENDPOINT_OUT)
                            endpoints.bulk_out = bEndpointAddress;
                    }
                    break;
                }
            }

            printf("\nClaiming the interface...\n");
            int interface = 0;
            if (libusb_claim_interface(handle, interface) != 0) {
                fprintf(stderr, "unable to claim the interface of the USB device\n");
            }

            printf("\nPower on CCID device...\n");
            const int timeout = 5000;
            unsigned char pwrOnCmd[10] = {0x62/*IccPowerOn*/, 0/*dwLength*/, 0x00, 0x00, 0x00,
                                          0x00/*slot number*/, 0x01/*sequance*/,  0x01/*5v*/, 0x00, 0x00};
            int pwrOnLength = 0;
            int e = libusb_bulk_transfer(handle, endpoints.bulk_out, pwrOnCmd, sizeof(pwrOnCmd), &pwrOnLength, timeout);
            printf("\nXfer returned with %d", e);

            printf("\nReading reply ATR...\n\n");
            const int maxAtrSize = 33;
            unsigned char atr[maxAtrSize] = {};
            int atrLength = 0;
            e = libusb_bulk_transfer(handle, endpoints.bulk_in, atr, sizeof(atr), &atrLength, timeout);
            printf("Xfer returned with %d\n", e);
            printf("\nATR:");
            printBytesHex(atr, atrLength);

            //controlUSB(handle, 0, 0xA1, 0x62, interface, atr, sizeof(atr), timeout);
            //printf("\nATR: %s", atr);

            printf("\nWriting data...\n");
            unsigned char command[] = {0x6F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x02/*sequance*/,
                                       0x00, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x00};
            int commandLength = 0;
            e = libusb_bulk_transfer(handle, endpoints.bulk_out, command, sizeof(command),
                                          &commandLength, timeout);
            printf("Xfer returned with %d", e);
            printf("\nSent %d bytes with data: \n", commandLength);
            printBytesHex(command, commandLength);

            printf("\nReading reply...\n\n");
            unsigned char reply[256] = {};
            int receivedLength = 0;
            e = libusb_bulk_transfer(handle, endpoints.bulk_in, reply, sizeof(reply),
                                     &receivedLength, timeout);
            printf("Xfer returned with %d\n", e);
            printf("\nReceived %d bytes with data: \n", receivedLength);
            printBytesHex(reply, receivedLength);

            printf("\nPower off CCID device...\n\n");
            unsigned char pwrOffCmd[10] = {0x63/*IccPowerOff*/, 0/*dwLength*/, 0x00, 0x00,
                                           0x00, 0x00/*slot number*/, 0x03/*sequance*/,  0x00, 0x00, 0x00};
            int pwrOffLength = 0;
            e = libusb_bulk_transfer(handle, endpoints.bulk_out, pwrOffCmd, sizeof(pwrOffCmd),
                                     &pwrOffLength, timeout);
            printf("Xfer returned with %d\n", e);

            printf("\nReading reply...\n\n");
            unsigned char replyPowerOff[256] = {};
            int receivedPorwerOffLength = 0;
            e = libusb_bulk_transfer(handle, endpoints.bulk_in, replyPowerOff, sizeof(replyPowerOff),
                                     &receivedPorwerOffLength, timeout);
            printf("Xfer returned with %d\n", e);

            printf("\nReleasing the interface...\n");
            if (libusb_release_interface(handle, 0) != 0) {
                fprintf(stderr, "unable to release interface of the USB device\n");
            }

            printf("\nClosing the USB device...\n");
            libusb_close(handle);
        }
	}
    if (!rutokenFound)
        printf("Rutoken not found!\n");
}

int main(void)
{
	printf("Test long transmit functions on Evotor, ver 1.0.0\n");
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int) cnt;

	print_devs(devs);
	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);
    printf("%s\n", "Complete");
	return 0;
}

