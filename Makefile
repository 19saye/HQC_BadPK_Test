INC_DIR := ../PrscHQC/include
SRC_DIR := ../PrscHQC/src
PQCLEAN_DIR  := vendor/pqclean/crypto_kem/hqc-128/clean
PQCOMMON_DIR := vendor/pqclean/common
MLDSA_DIR    := vendor/pqclean/crypto_sign/ml-dsa-44/clean
OTA_DIR      := demo/ota_update
SUPPORT_DIR := support
DEMO_DIR := demo/failsafe_car

CC := clang
CFLAGS := -O3 -std=c11 -Wall -Wextra -Wno-unused-parameter

.PHONY: mock
mock: badpk

badpk: experiments/badpk.c
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ \
		$(SRC_DIR)/channel.c $(SRC_DIR)/crypto.c $(SRC_DIR)/packet.c \
		-o $@

.PHONY: real
real: badpk_real
badpk_real: experiments/badpk_real.c $(SUPPORT_DIR)/randombytes.c
	$(CC) $(CFLAGS) -I$(PQCLEAN_DIR) -I$(PQCOMMON_DIR) -I$(SUPPORT_DIR) $^ \
		$(PQCOMMON_DIR)/randombytes.c \
		$(PQCLEAN_DIR)/code.c \
		$(PQCLEAN_DIR)/fft.c \
		$(PQCLEAN_DIR)/gf.c \
		$(PQCLEAN_DIR)/gf2x.c \
		$(PQCLEAN_DIR)/hqc.c \
		$(PQCLEAN_DIR)/kem.c \
		$(PQCLEAN_DIR)/parsing.c \
		$(PQCLEAN_DIR)/vector.c \
		$(PQCLEAN_DIR)/reed_muller.c \
		$(PQCLEAN_DIR)/reed_solomon.c \
		$(PQCLEAN_DIR)/shake_ds.c \
		$(PQCLEAN_DIR)/shake_prng.c \
		$(PQCOMMON_DIR)/fips202.c \
		$(PQCOMMON_DIR)/sp800-185.c \
		-o $@

.PHONY: demo
demo: $(DEMO_DIR)/failsafe_demo

$(DEMO_DIR)/failsafe_demo: $(DEMO_DIR)/main_demo.c $(DEMO_DIR)/aes_gcm.c $(DEMO_DIR)/packet_format.c $(DEMO_DIR)/failsafe_ecu.c $(SUPPORT_DIR)/randombytes.c
	$(CC) $(CFLAGS) \
		-I$(PQCLEAN_DIR) -I$(PQCOMMON_DIR) -I$(SUPPORT_DIR) -I$(DEMO_DIR) \
		$^ \
		$(PQCOMMON_DIR)/randombytes.c \
		$(PQCLEAN_DIR)/code.c \
		$(PQCLEAN_DIR)/fft.c \
		$(PQCLEAN_DIR)/gf.c \
		$(PQCLEAN_DIR)/gf2x.c \
		$(PQCLEAN_DIR)/hqc.c \
		$(PQCLEAN_DIR)/kem.c \
		$(PQCLEAN_DIR)/parsing.c \
		$(PQCLEAN_DIR)/vector.c \
		$(PQCLEAN_DIR)/reed_muller.c \
		$(PQCLEAN_DIR)/reed_solomon.c \
		$(PQCLEAN_DIR)/shake_ds.c \
		$(PQCLEAN_DIR)/shake_prng.c \
		$(PQCOMMON_DIR)/fips202.c \
		$(PQCOMMON_DIR)/sp800-185.c \
		-lssl -lcrypto \
		-o $@

.PHONY: ota
ota: $(OTA_DIR)/ota_demo

$(OTA_DIR)/ota_demo: $(OTA_DIR)/ota_demo.c
	$(CC) $(CFLAGS) \
		-I$(MLDSA_DIR) -I$(PQCOMMON_DIR) \
		$^ \
		$(MLDSA_DIR)/ntt.c \
		$(MLDSA_DIR)/packing.c \
		$(MLDSA_DIR)/poly.c \
		$(MLDSA_DIR)/polyvec.c \
		$(MLDSA_DIR)/reduce.c \
		$(MLDSA_DIR)/rounding.c \
		$(MLDSA_DIR)/sign.c \
		$(MLDSA_DIR)/symmetric-shake.c \
		$(PQCOMMON_DIR)/randombytes.c \
		$(PQCOMMON_DIR)/fips202.c \
		$(PQCOMMON_DIR)/sp800-185.c \
		-o $@

.PHONY: clean
clean:
	rm -f badpk badpk_real $(DEMO_DIR)/failsafe_demo $(OTA_DIR)/ota_demo
