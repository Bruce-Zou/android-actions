##ActduinoTest drivers

1.��ActduinoTest\drivers\actduinotest������kernel\driversĿ¼
2.�޸ı����ļ�
  kernel\drivers\Kconfig,���source "drivers/actduinotest/Kconfig"
  kernel\drivers\Makefile���obj-y    += actduinotest/
3.��kernel\arch\arm\configs\actduino_s500_defconfig������kernel/.config
4.����
  ��kernelĿ¼��
  $make menuconfig
  $make modules
  
  ��������
  kernel/drivers/actduinotest/i2c_test/i2c_owl_test.ko
  
kernel/drivers/actduinotest/spi_test/spi_owl_test.ko

  kernel/drivers/actduinotest/gpio_test/gpio_owl_test.ko
  
kernel/drivers/actduinotest/adc_test/adc_owl_test.ko

  