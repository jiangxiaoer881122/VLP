# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/soft_iic.c)
target_sources(app PRIVATE src/imu_bag.c)
target_sources(app PRIVATE src/timer.c)
target_sources(app PRIVATE src/adc.c)
target_sources(app PRIVATE src/iic_twi.c)
target_sources(app PRIVATE src/uart.c)
target_sources(app PRIVATE src/fft.c)
target_sources(app PRIVATE src/ble.c)
target_sources(app PRIVATE src/ads1015.c)
target_sources(app PRIVATE src/ads1220.c)