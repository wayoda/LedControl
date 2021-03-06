#pragma once

///This check allows the user to define their own PRINTLN method.
///By doing that more boards and platforms can be supported even
///if they are not supported by the Arduino IDE.
#ifndef PRINTLN
#if (ARDUINO >= 100)
#include <Arduino.h>
#define PRINTLN(x) Serial.println(x)
#endif
#endif

#define PRINTLN_IF(condition,x) if(condition){PRINTLN(x);}

namespace sakurajin {

    /**
     * \~english
     * @brief This class is used to handle the configuration of the sakurajin::LedController
     *
     * @tparam columns The number of columns this configuration has.
     * @tparam rows The number of rows this configuration has.
     *
     * \~german
     * @brief Diese Klasse wird zur Konfiguration des sakurajin::LedController verwendet.
     *
     * @tparam columns Die Anzahl der Spalten, die diese Konfiguration hat.
     * @tparam rows Die Anzahl der Zeilen, die diese Konfiguration hat.
     */
    template <size_t columns, size_t rows>
    class controller_configuration {
      public:

        /**
         * \~english
         * @brief The pin for the data transfer signal (MOSI on board and DIN on Matrix).
         * @warning if this is 0 and useHardwareSpi is false the initilization will fail
         *
         * \~german
         * @brief Der pin für das data transfer Signal (MOSI am Board und DIN an der Matrix).
         * @warning Wenn das 0 ist und useHardwareSpi false ist, ist die Konfiguration ungültig.
         */
        unsigned int SPI_MOSI = 0;

        /**
         * \~english
         * @brief The pin for the chip select signal (CS).
         * @warning this has to be set if it is 0 the initlization will fail.
         *
         * \~german
         * @brief Der pin für das chip select Signal (CS).
         * @warning Wenn das 0 ist und useHardwareSpi false ist, ist die Konfiguration ungültig.
         */
        unsigned int SPI_CS = 0;

        /**
         * \~english
         * @brief The pin for the clock signal (CLK).
         * @warning if this is 0 and useHardwareSpi is false the initilization will fail
         *
         * \~german
         * @brief Der pin für das clock Signal (CLK).
         * @warning Wenn das 0 ist und useHardwareSpi false ist, ist die Konfiguration ungültig.
         */
        unsigned int SPI_CLK = 0;

        /**
         * \~english
         * @brief true if you want to use hardware SPI (view https://www.arduino.cc/en/Reference/SPI for pin config). While this is a
         * lot faster you cannot use every pin for the MOSI and CLK signal. SPI_MOSI
         * and SPI_CLK will be set automatically if this is true.
         *
         * \~german
         * @brief true falls hardware SPI genutzt werden soll (schaue https://www.arduino.cc/en/Reference/SPI für die Pin Konfiguration).
         * Diese Einstellung ist zwar sehr viel schneller, dafür kann nicht jeder PIN für MOSI und CLK verwendet werden.
         * SPI_MOSI und SPI_CLK werden automatisch gesetzt wenn diese Variable true ist
         */
        bool useHardwareSpi = false;

        /**
         * \~english
         * @brief The current brightness level of the leds.
         * @note The value needs to be between 0 and 15 (inclusive bounds).
         *
         * \~german
         * @brief Die aktuelle Helligkeit der LEDs.
         * @note Der Wert muss zwischen 0 und 15 sein (inklusive Grenzen).
         */
        unsigned int IntensityLevel = 1;

        /**
         * \~english
         * @brief This Arrays specifies which Pin to use for each row.  @ref multi_row
         * @warning Each index should have a unique number or you will encounter strange behaviour.
         *
         *
         * \~german
         * @brief Dieses Array bestimmt welcher CS Pin für welche Reihe verwendet wird. @ref multi_row
         * @warning Jeder Eintrag sollte eine einzigartige Nummer haben, wenn nicht kann es zu Problemen führen.
         */
        unsigned int row_SPI_CS[rows];

        /**
         * \~english
         * @brief Only send data if something changed if true.
         * If this is true, new data will only be sent if the data given differes from the data internally stored.
         *
         * \~german
         * @brief Nur Daten übertragen wenn sich etwas geändert hat.
         * Wenn dies auf true gesetzt ist, werden Daten nur übertragen, wenn diese von den gespeicherten abweichen.
         */
        bool onlySendOnChange = true;

        /**
         * \~english
         * @brief The speed which with the hardware spi should transfer the data to the matrix.
         *
         * \~german
         * @brief Die Geschwindigkeit mit der hardware SPI die Daten übertragen soll.
         */
        uint64_t spiTransferSpeed = 8000000;

        /**
         * \~english
         * @brief if this is set to true, debug output will be printed.
         *
         * \~german
         * @brief Wenn das true ist, werden debug informationen ausgegeben.
         */
        bool debug_output = false;

        /**
         * \~english
         * @brief set to false if each of your rows has a dedicated CS pin.
         * By default this is true and it is assumed that all Segments are connected in series.
         *
         * \~german
         * @brief Setze das auf false wenn jede Zeile einen eigenen CS Pin besitzt.
         * Standardmäßig ist es auf true gesetzt und es wird davon ausgegangen, dass alle Segemnte in Reihe geschaltet sind.
         */
        bool virtual_multi_row = true;

        /**
         * \~english
         * @brief returns the total number of segments (rows*columns)
         *
         * @return unsigned int the number of segments
         *
         * \~german
         * @brief Gibt die Anzahl der Segmente zurück (Zeilen*Spalten).
         *
         * @return unsigned int Die Anzahl der Segmente.
         */
        unsigned int SegmentCount() const noexcept {
            return rows*columns;
        }

        /**
         * \~english
         * @brief check of this configuration is valid
         *
         * @return true the configuration is valid
         * @return false the configuration is not valid
         *
         *
         * \~german
         * @brief Prüft ob diese Konfiguration gültig ist.
         *
         * @return true Die Konfiguration ist gültig.
         * @return false Die Konfiguration ist ungültig.
         */
        bool isValid() const noexcept {
            return isValidConfig(*this);
        }

        /**
         * \~english
         * @brief This function returns the row a given segment is in.
         *
         * @param segmentNumber The index of the segment which you want the row of.
         * @return unsigned int The row the given segment is in.
         *
         * \~german
         * @brief Diese Funktion berechnet die Zeile aus der Segmentnummer.
         *
         * @param segmentNumber Die Segmentnummer des gesuchten Segments.
         * @return unsigned int Die Zeile in der das Segment ist.
         */
        unsigned int getRow(unsigned int segmentNumber) const noexcept {
            unsigned int row = 0;
            if (rows != 0 && columns != 0) {
                row = segmentNumber / columns;
            }

            if(row >= rows) {
                row = 0;
            }

            return row;
        }

        /**
         * \~english
         * @brief This function returns the column a given segment is in.
         *
         * @param segmentNumber The index of the segment which you want the column of.
         * @return unsigned int The column the given segment is in.
         *
         * \~german
         * @brief Diese Funktion berechnet die Spalte aus der Segmentnummer.
         *
         * @param segmentNumber Die Segmentnummer des gesuchten Segments.
         * @return unsigned int Die Spalte in der das Segment ist.
         */
        unsigned int getColumn(unsigned int segmentNumber) const noexcept {
            unsigned int col = 0;
            if (rows != 0 && columns != 0) {
                col = segmentNumber % columns;
            }

            return col;
        }

        /**
         * \~english
         * @brief Get the length each row has.
         *
         * @return unsigned int the length of each row
         *
         * \~german
         * @brief Gibt die Länge einer Zeile zurück.
         *
         * @return unsigned int Die Länge einer Zeile
         */
        unsigned int getRowLen() const noexcept {
            return columns;
        }

        /**
         * \~english
         * @brief Calculate the segment Number/index from its coordinates.
         *
         * @param column The column the segment is in
         * @param row The row the segment is in
         * @return unsigned int the segment number of that segment
         *
         * \~german
         * @brief Berechne die Segmentnummer/-index aus den Coordinaten.
         *
         * @param column Die Spalte in der das Segment ist
         * @param row Die Zeile in der das Segment ist
         * @return unsigned int Die Segmentnummer des segments
         */
        unsigned int getSegmentNumber(unsigned int column, unsigned int row) const noexcept {
            row %= rows;
            column %= columns;
            return row * columns + column;
        }

        /**
         * \~english
         * @brief This function returns a copy of this configuration
         *
         * @return controller_configuration a copy of the configuration
         *
         * \~german
         * @brief Diese Funktion gibt eine Kopie dieser Konfiguration zurück.
         *
         * @return controller_configuration Eine Kopie der Konfiguration
         */
        controller_configuration<columns,rows> copy() const noexcept {
            controller_configuration<columns,rows> conf;
            conf.IntensityLevel = this->IntensityLevel;
            conf.onlySendOnChange = this->onlySendOnChange;
            for(unsigned int i = 0; i < this->rows; i++) {
                conf.row_SPI_CS[i] = this->row_SPI_CS[i];
            }
            conf.SPI_CLK = this->SPI_CLK;
            conf.SPI_CS = this->SPI_CS;
            conf.SPI_MOSI = this->SPI_MOSI;
            conf.useHardwareSpi = this->useHardwareSpi;
            return conf;
        }

        /**
         * \~english
         * @brief check if a given configuragtion is valid
         *
         * @param conf the configuration that should be checked
         * @return true the configuration is valid
         * @return false the configuration is not valid
         *
         * \~german
         * @brief Prüfe ob die gegebene Konfiguration gültig ist.
         *
         * @param conf Die Konfiguration die geprüft werden soll.
         * @return true Die Konfiguration ist gültig.
         * @return false Die Konfiguration ist ungültig.
         */
        static bool isValidConfig(const controller_configuration<columns,rows> &conf) noexcept {
            //check if the dimenstions are valid
            if (rows == 0 || columns == 0) {
                return false;
            }

            // checking the clk amd mosi pins
            if (!conf.useHardwareSpi) {
                if (conf.SPI_CLK == 0) {
                    PRINTLN_IF(conf.debug_output, "No CLK Pin given. Specify one or set useHardwareSpi to true");
                    return false;
                }

                if (conf.SPI_MOSI == 0) {
                    PRINTLN_IF(conf.debug_output, "No MOSI Pin given. Specify one or set useHardwareSpi to true");
                    return false;
                }
            }

            // checking the cs pin(s)
            if (conf.SPI_CS == 0 && conf.row_SPI_CS == nullptr) {
                PRINTLN_IF(conf.debug_output, "No CS Pin given");
                return false;
            }

            if (!conf.virtual_multi_row || conf.SPI_CS == 0) {
                for(unsigned int i = 0; i < rows; i++) {
                    if(conf.row_SPI_CS[i] == 0) {
                        PRINTLN_IF(conf.debug_output, "Invalid value in row_SPI_CS found. 0 is not allowed.");
                        return false;
                    }
                    for(unsigned int j = 0; j < i; j++) {
                        if(conf.row_SPI_CS[i] == conf.row_SPI_CS[j]) {
                            PRINTLN_IF(conf.debug_output, "None unique value in row_SPI_CS found! Each row needs its own pin or consider enabling virtual_multi_row.");
                            return false;
                        }
                    }
                }
            }


            return true;
        }
    };
}
