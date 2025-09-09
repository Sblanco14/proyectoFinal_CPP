# Proyecto Práctico – Sistemas de Monitoreo Biomédico

> **Curso:** Programación avanzada  
> **Autores:** Santiago Blanco – Paula Sofía Niño  
> **Repositorio:** `proyectoFinal_CPP`  
> **Carpeta raíz del código en el repo:** `proyecto/`  
> **Lenguaje:** C++  
> **Plataforma objetivo:** Linux / Raspberry Pi

---

## Objetivo
Sistema en C++ que procesa lecturas biomédicas de archivos binarios **`.bsf`**, aplica **umbrales** de configuración y **datos de pacientes**, detecta **anomalías**, genera **reportes de texto**, y exporta pacientes con **ECG anómalo** a un binario.

---

## Estructura del repositorio

```
proyecto/
├─ libs/                       # 21 headers (estructuras y funciones)
│  ├─ tamanos.h                # Constantes de tamaño (ID, fecha, rutas)
│  ├─ lectura.h                # struct Lectura { tipo, valor, sistólica, diastólica }
│  ├─ umbral.h                 # struct Umbral { tipoSensor[16], min, max }
│  ├─ medicion.h               # struct Medicion { idPaciente, fechaHora, numLecturas, Lectura* }
│  ├─ maquina.h                # struct MaquinaUCI { id, numMediciones, Medicion* }
│  ├─ sala.h                   # struct Sala { idSala, numMaquinas, MaquinaUCI* }
│  ├─ paciente.h               # struct Paciente { campos del CSV }
│  ├─ archivo_pacientes.h      # struct ArchivoPacientes { numPacientes, Paciente* }
│  ├─ funcionesConfig.h        # parseLineaConfiguracion(linea) → Umbral
│  ├─ funcionesPacientes.h     # parseLineaPaciente(linea) → Paciente
│  ├─ cargarDatos.h            # cargarUmbrales(), cargarPacientes()
│  ├─ funcionesFecha.h         # compararFechas("DD/MM/AAAA HH:MM:ss.sss")
│  ├─ ordenar.h                # ordenarMedicionesPorFecha() (inserción)
│  ├─ anomalias.h              # esAnomalaNoECG() para T/P/O
│  ├─ anomalias_ecg.h          # minMaxECGEnMaquina(), ecgAnomaloPacienteEnMaquina()
│  ├─ estadisticas.h           # acumuladores y promedio (min/max/prom)
│  ├─ reportes.h               # generarReporteMedicionesPaciente()
│  ├─ leerBSF.h                # leerArchivoBSF(), liberarSala()
│  ├─ export_ecg.h             # exportarPacientesECGAnomalos() (BIN ECG)
│  └─ reporteAnomalias.h       # generarReporteAnomalias() (txt global)
│
├─ data/                       # Archivos de entrada para pruebas
│  ├─ configuracion.txt
│  ├─ pacientes.csv
│  ├─ pacientes_small.csv
│  ├─ patient_readings_simulation 1.bsf
│  ├─ patient_readings_simulation_small 1.bsf
│  ├─ patient_readings_simulation_tiny 1.bsf
│  └─ *.txt de simulación
│
├─ main.cpp                    # Programa principal con menú
├─ "validate_export_file (1).cpp"  # Validador/convertidor de BIN ECG → TXT
└─ (salidas generadas)         # anomalias.txt, pacientes_ecg_anomalos.dat, etc.
```
---

## Compilación

Compila con:
```bash
# 1) Instalar herramientas básicas
sudo apt update
sudo apt install -y git build-essential make

# 2) Clonar el repo público
git clone https://github.com/Sblanco14/proyectoFinal_CPP.git
cd ProyectoFinal_C++

# 3) Compilar (genera 'app' y 'validacion' en la raíz)
make clean
make

# 4) Ir a la carpeta que contiene 'data' y ejecutar el programa
cd proyecto
../app

# 1  (Enter)  -> Cargar configuración y pacientes
# 2  (Enter)  -> Leer .bsf
# 3  (Enter)  -> Generar reporte de anomalías (T/P/O)
# 4  (Enter)  -> Exportar ECG anómalo (BIN)
# 0  (Enter)  -> Salir

# 5) Convertir el BIN de ECG a TXT legible
../validacion

# 6) Mostrar rápidamente los resultados
ls -l anomalias.txt pacientes_ecg_anomalos.dat validation_ecg_anomalies.txt
head -n 20 anomalias.txt
head -n 20 validation_ecg_anomalies.txt
```

---

## Ejecución

### 1) Programa principal (`app`)
```bash
./app
```
**Menú:**
1. Cargar **configuración** (`data/configuracion.txt`) y **pacientes** (`data/pacientes.csv`).  
2. Leer archivo **BSF** (`data/patient_readings_simulation 1.bsf`).  
3. Generar **reporte de anomalías** global (`anomalias.txt`) – *excluye ECG*.
4. Exportar **pacientes con ECG anómalo** a binario (`pacientes_ecg_anomalos.dat`).
0. Salir.

> **Orden recomendado:** 1 → 2 → 3/4.  

**Archivos generados:**
- `anomalias.txt` → listado de lecturas **T/P/O anómalas**, agrupadas por paciente, orden cronológico.
- `pacientes_ecg_anomalos.dat` → BIN con pacientes que presentan **ECG anómalo** (formato especificado en la guía del proyecto).

### 2) Validador/convertidor del BIN ECG (`validacion`)
Convierte `pacientes_ecg_anomalos.dat` a un TXT legible para verificación manual.

```bash
./validacion
```
Genera: `validation_ecg_anomalies.txt`.

---

## Resumen funcional
- **Carga de configuración** (`cargarUmbrales`) → lee `configuracion.txt`, almacena en un arreglo dinámico de `Umbral`.
- **Carga de pacientes** (`cargarPacientes`) → lee `pacientes.csv`, tokeniza por `;`, crea `ArchivoPacientes` con arreglo dinámico de `Paciente`.
- **Lectura BSF** (`leerArchivoBSF`) → carga una `Sala` con `MaquinaUCI`→`Medicion`→`Lectura` desde un archivo `.bsf` (manejo de `new[]`/`delete[]`).
- **Orden cronológico** (`ordenarMedicionesPorFecha`) → inserción basada en `compararFechas`.
- **Detección de anomalías T/P/O** (`esAnomalaNoECG`) → compara con umbrales de `Umbral`.
- **Reporte global** (`generarReporteAnomalias`) → `anomalias.txt` con T/P/O anómalas por paciente.
- **Reporte por paciente** (`generarReporteMedicionesPaciente`) → ordena por fecha, agrupa por sensor, marca *NORMAL/ANOMALA* y calcula **min/max/prom**.
- **Anomalía ECG** (`minMaxECGEnMaquina` + `ecgAnomaloPacienteEnMaquina`) → regla:  
  `|minECG| + |maxECG| > |umbral.min| + |umbral.max|`  ⇒ anómalo.
- **Exportar ECG anómalo** (`exportarPacientesECGAnomalos`) → BIN con **ID (11 bytes)**, **nº mediciones ECG (4 bytes)**, y luego **(fecha 24 bytes + valor double 8 bytes)** por medición.
- **Validación BIN** (`ValidateEcgExportFile`) → genera `validation_ecg_anomalies.txt` legible.

---

## Supuestos y validaciones
- `leerArchivoBSF` valida que `numLecturas` sea razonable (p. ej., corta si > 10000).
- Todos los **arreglos son dinámicos**; se libera memoria con `liberarSala` y `delete[]` cuando corresponde.

---

## Pruebas en PC y Raspberry Pi
1. **Cargar configuración y pacientes** (opción 1) → verificar conteos.
2. **Cargar BSF** (opción 2) → usar `data/patient_readings_simulation 1.bsf` o los archivos `small/tiny`.
3. **Generar anomalias** (opción 3) → revisar `anomalias.txt` (formato legible para médico).
4. **Exportar ECG anómalo** (opción 4) → revisar `pacientes_ecg_anomalos.dat` y convertir con `./validacion`.

---

## Créditos
- **Autores:** Santiago Blanco – Paula Sofía Niño  
- **Curso:** Programación avanzada
