APPS_FOLDERS=tcp-cli-srv tcp-chat udp-cli-srv udp-snd-rcv tcp-http-srv

all: 
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make );)

clean:
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make clean);)

