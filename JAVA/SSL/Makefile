APPS_FOLDERS=check-TLS https-server-ajax-voting tcp-cli-srv

all: 
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make );)

clean:
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make clean);)

