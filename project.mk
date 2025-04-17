# Add inputs and outputs from these tool invocations to the build variables
OUT_DIR += \
/$(SRC_PATH) \
/$(SRC_PATH)/zcl \
/$(SRC_PATH)/epaper \
/$(SRC_PATH)/bme280 \
/$(SRC_PATH)/common
 
OBJS += \
$(OUT_PATH)/$(SRC_PATH)/common/main.o \
$(OUT_PATH)/$(SRC_PATH)/zcl/zcl_concentration_measurement.o \
$(OUT_PATH)/$(SRC_PATH)/zcl/zcl_relative_humidity_measurement.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/epaper.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font12.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font13.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font16.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font24.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font26.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font32.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font58.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font68.o \
$(OUT_PATH)/$(SRC_PATH)/epaper/font8.o \
$(OUT_PATH)/$(SRC_PATH)/bme280/bme280.o \
$(OUT_PATH)/$(SRC_PATH)/app_arith64.o \
$(OUT_PATH)/$(SRC_PATH)/app_i2c.o \
$(OUT_PATH)/$(SRC_PATH)/app_bme280.o \
$(OUT_PATH)/$(SRC_PATH)/app_epd.o \
$(OUT_PATH)/$(SRC_PATH)/app_sensors.o \
$(OUT_PATH)/$(SRC_PATH)/app_config.o \
$(OUT_PATH)/$(SRC_PATH)/app_endpoint_cfg.o \
$(OUT_PATH)/$(SRC_PATH)/app_button.o \
$(OUT_PATH)/$(SRC_PATH)/app_led.o \
$(OUT_PATH)/$(SRC_PATH)/app_utility.o \
$(OUT_PATH)/$(SRC_PATH)/app_time.o \
$(OUT_PATH)/$(SRC_PATH)/app_main.o \
$(OUT_PATH)/$(SRC_PATH)/zb_appCb.o \
$(OUT_PATH)/$(SRC_PATH)/zcl_appCb.o

#$(OUT_PATH)/$(SRC_PATH)/app_co2sensor_model1.o \
#$(OUT_PATH)/$(SRC_PATH)/app_co2sensor.o \

# Each subdirectory must supply rules for building sources it contributes
$(OUT_PATH)/$(SRC_PATH)/%.o: $(SRC_PATH)/%.c 
	@echo 'Building file: $<'
	@$(CC) $(GCC_FLAGS) $(INCLUDE_PATHS) -c -o"$@" "$<"


