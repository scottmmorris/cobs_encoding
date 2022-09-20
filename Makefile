test_cobs: transcode_tests.c cobs.c
	gcc -o transcode_test transcode_tests.c cobs.c -I .
