APPS_FOLDERS=tcp-cli-srv tcp-chat udp-cli-srv udp-snd-rcv udp-cli-srv-soma

all: 
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make );)

clean:
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make clean);)

