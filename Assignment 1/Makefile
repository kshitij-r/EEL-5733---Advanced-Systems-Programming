all:	email_filter calender_filter location_updater

email_filter: email_filter.c
	gcc -o email_filter email_filter.c
calender_filter: calender_filter.c
	gcc -o calender_filter calender_filter.c
location_updater: location_updater.c
	gcc -o location_updater location_updater.c
clean:
	rm -rf email_filter calender_filter location_updater
