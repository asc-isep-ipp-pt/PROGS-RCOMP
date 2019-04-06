
APPS_FOLDERS=tcp-chat tcp-cli-srv udp-chat udp-cli-srv http-server-form-file-upload http-server-ajax-voting http-server-chat SSL

all: 
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make );)

clean:
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make clean);)

