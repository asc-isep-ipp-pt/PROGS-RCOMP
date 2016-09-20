APPS_FOLDERS=tcp-chat tcp-cli-srv tcp-http-srv udp-chat udp-cli-srv

all: 
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make );)

clean:
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make clean);)

