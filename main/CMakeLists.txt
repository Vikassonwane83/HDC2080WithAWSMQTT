idf_component_register(SRCS "awsMQTT.c" "fileSystem.c" "main.c" "wifiConfig.c" "tempSensor.c" "logic.c"
                    INCLUDE_DIRS ".")


# set(COMPONENT_ADD_INCLUDEDIRS ".")     
# register_component()               
# set(EXTRA_COMPONENT_DIRS "C:/Users/idea/esp/esp-idf/components/esp-aws-iot")


# target_add_binary_data(${COMPONENT_TARGET} "certs/aws-root-ca.pem" TEXT)
# target_add_binary_data(${COMPONENT_TARGET} "certs/certificate.pem.crt" TEXT)
# target_add_binary_data(${COMPONENT_TARGET} "certs/private.pem.key" TEXT)
