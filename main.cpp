#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#include <iostream>

#include <usb.h>

using namespace std;

int deviceCounter = 0;
int device;

void debugmsg(std::string msg) {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);

  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  cout << buf << " - "<< msg << "\n";
}

int main(int argc, char *argv[]) {
	std::string strargp, strargd;
	debugmsg("\n----------------------------\nUSBReset by DO2JMG 2021\n----------------------------\n");

    if (argc > 2) {
		debugmsg("parameters ok!");
		strargp = argv[1];
		strargd = argv[2];
		if (strargp != "-d") { debugmsg("No parameter -d"); return 0; }
	}

    struct usb_bus *bus;
    struct usb_device *dev;
	std::string strDevice, strBus, strReset;

    usb_init();
    usb_find_busses();
    usb_find_devices();
    for (bus = usb_busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next){
			if (dev->descriptor.idProduct == 0x2838) {
				deviceCounter++;
				if (deviceCounter == (std::stoi(strargd)+1)) {
					strDevice = dev->filename;
					strBus = bus->dirname;
					debugmsg("Device found at Bus : " + strBus + " and Device : " + strDevice);
				}
			}
        }
	}
	if (strBus.length() == 0) {
		debugmsg("No device found!");
		return 0;
	}

	strReset = "/dev/bus/usb/"+ strBus+"/"+strDevice;
	debugmsg("Reset String is :");
	debugmsg(strReset);
	
	const char *filename;
	int fd;
	int result;

	filename = strReset.c_str();

	fd = open(filename, O_WRONLY);
	result = ioctl(fd, USBDEVFS_RESET, 0);
	close(fd);
	if (result == 0) {
		debugmsg("USB reset Ok!");
	} else {
		debugmsg("USB Reset failed!");
	}

	return 0;
}