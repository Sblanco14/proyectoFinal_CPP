#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

/**
 * Función para validar y convertir el archivo binario de anomalías ECG a formato de texto
 * @param binary_filename Nombre del archivo binario (.dat) a leer
 * @param text_filename Nombre del archivo de texto (.txt) a generar
 * @return true si la validación fue exitosa, false en caso contrario
 */

bool ValidateEcgExportFile(const char* binary_filename, const char* text_filename) {
    std::ifstream binary_file(binary_filename, std::ios::in | std::ios::binary);
    
    if (!binary_file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo binario " << binary_filename << std::endl;
        return false;
    }

    std::ofstream text_file(text_filename);
    
    if (!text_file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de texto " << text_filename << std::endl;
        binary_file.close();
        return false;
    }

    text_file << "=== VALIDACION DEL ARCHIVO DE ANOMALIAS ECG ===" << std::endl;
    text_file << "Archivo fuente: " << binary_filename << std::endl;
    text_file << "===============================================" << std::endl << std::endl;

    int patient_count = 0;
    int total_anomalies = 0;

    // Leer el archivo binario hasta el final
    while (binary_file.peek() != EOF) {
        char patient_id_bytes[11];
        
        // Leer ID del paciente (11 bytes)
        binary_file.read(patient_id_bytes, sizeof(patient_id_bytes));
        
        if (binary_file.gcount() != sizeof(patient_id_bytes)) {
            if (binary_file.gcount() == 0) {
                break; // Fin del archivo
            }
            std::cerr << "Error: Lectura incompleta del ID del paciente" << std::endl;
            text_file << "ERROR: Lectura incompleta del ID del paciente" << std::endl;
            binary_file.close();
            text_file.close();
            return false;
        }

        // Convertir ID del paciente a string
        char patient_id_str[12] = {0}; // +1 para null terminator
        strncpy(patient_id_str, patient_id_bytes, sizeof(patient_id_bytes));
        
        // Leer número de anomalías (4 bytes)
        int num_anomalies;
        binary_file.read(reinterpret_cast<char*>(&num_anomalies), sizeof(num_anomalies));
        
        if (binary_file.gcount() != sizeof(num_anomalies)) {
            std::cerr << "Error: Lectura incompleta del número de anomalías" << std::endl;
            text_file << "ERROR: Lectura incompleta del numero de anomalias" << std::endl;
            binary_file.close();
            text_file.close();
            return false;
        }

        patient_count++;
        total_anomalies += num_anomalies;

        text_file << "PACIENTE #" << patient_count << std::endl;
        text_file << "  ID: " << patient_id_str << std::endl;
        text_file << "  Numero de anomalias ECG: " << num_anomalies << std::endl;
        text_file << "  Anomalias:" << std::endl;

        // Leer cada anomalía
        for (int i = 0; i < num_anomalies; i++) {
            char date_time[24];
            double ecg_value;

            // Leer fecha/hora (24 bytes)
            binary_file.read(date_time, sizeof(date_time));
            
            if (binary_file.gcount() != sizeof(date_time)) {
                std::cerr << "Error: Lectura incompleta de fecha/hora para anomalía " << i+1 << std::endl;
                text_file << "    ERROR: Lectura incompleta de fecha/hora para anomalia " << i+1 << std::endl;
                binary_file.close();
                text_file.close();
                return false;
            }

            // Leer valor ECG (8 bytes)
            binary_file.read(reinterpret_cast<char*>(&ecg_value), sizeof(ecg_value));
            
            if (binary_file.gcount() != sizeof(ecg_value)) {
                std::cerr << "Error: Lectura incompleta del valor ECG para anomalía " << i+1 << std::endl;
                text_file << "    ERROR: Lectura incompleta del valor ECG para anomalia " << i+1 << std::endl;
                binary_file.close();
                text_file.close();
                return false;
            }

            // Convertir fecha/hora a string legible
            char date_time_str[25] = {0};
            strncpy(date_time_str, date_time, sizeof(date_time));

            text_file << "    Lectura: " << i+1 << ":" << std::endl;
            text_file << "      Fecha/Hora: " << date_time_str << std::endl;
            text_file << "      Valor ECG: " << std::fixed << std::setprecision(6) << ecg_value << std::endl;
        }
        
        text_file << std::endl;
    }

    // Escribir resumen final
    text_file << "=== RESUMEN DE VALIDACION ===" << std::endl;
    text_file << "Total de pacientes con anomalias ECG: " << patient_count << std::endl;
    text_file << "Total de lecturas: " << total_anomalies << std::endl;
    text_file << "Validacion completada exitosamente." << std::endl;

    binary_file.close();
    text_file.close();

    std::cout << "Validacion completada exitosamente." << std::endl;
    std::cout << "Archivo de texto generado: " << text_filename << std::endl;
    std::cout << "Pacientes procesados: " << patient_count << std::endl;
    std::cout << "Total de anomalias: " << total_anomalies << std::endl;

    return true;
}

/**
 * Función principal para ejecutar la validación
 */
int main() {
    const char* binary_filename = "pacientes_ecg_anomalos.dat";
    const char* text_filename = "validation_ecg_anomalies.txt";

    std::cout << "Iniciando validacion del archivo de exportacion ECG..." << std::endl;
    
    if (ValidateEcgExportFile(binary_filename, text_filename)) {
        std::cout << "Proceso de validacion completado con exito." << std::endl;
        return 0;
    } else {
        std::cerr << "Error durante el proceso de validacion." << std::endl;
        return 1;
    }
}
