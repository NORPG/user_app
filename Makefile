CC = arm-linux-gnueabihf-gcc

.PHONY: clean
user_app: user_app.c
	$(CC) -std=gnu11 -o user_app user_app.c
clean:
	rm -rf user_app
