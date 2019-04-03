APPS_FOLDERS=check-TLS tcp-cli-srv https-server-ajax-voting

all: 
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make );)

clean:
	 $(foreach AF,$(APPS_FOLDERS),(cd $(AF); make clean);)

