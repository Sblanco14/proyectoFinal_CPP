# Proyecto Práctico – Sistemas de Monitoreo Biomédico

> **Curso:** Programación avanzada  
> **Autores:** Santiago Blanco – Paula Sofía Niño  
> **Repositorio:** `ProyectoFinal_C++` (público)  
> **Carpeta raíz del código en el repo:** `proyecto/`  
> **Lenguaje:** C++ (C++14 recomendado)  
> **Plataforma objetivo:** Linux / Raspberry Pi

---

## 🎯 Objetivo
Sistema en C++ que procesa lecturas biomédicas de archivos binarios **`.bsf`**, aplica **umbrales** de configuración y **datos de pacientes**, detecta **anomalías**, genera **reportes de texto**, y exporta pacientes con **ECG anómalo** a un binario.

**Restricciones clave:**
- Sin librerías externas; solo estándar de C++.
- **Sin STL avanzada** ni `std::vector`. Se usan **arreglos dinámicos** manuales (`new[]/delete[]`).
- `std::string` únicamente como apoyo para separar/parsear texto.

---

## 📁 Estructura del repositorio

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

> **Importante:** El repo final usa **`proyecto/` (minúscula)**. Si trabajas en otro entorno donde la carpeta tenga mayúscula, asegura que las **rutas relativas** en el código sigan apuntando a `data/`.

---

## ⚙️ Requisitos
- **Compilador:** `g++` (C++14 recomendado). También funciona `clang++`.
- **SO:** Linux (probado en entorno tipo PC). Objetivo: Raspberry Pi (ARM).
- **Dependencias externas:** Ninguna.

### Flags sugeridas
Para desarrollo/entrega educativa:
```
-std=c++14 -O2 -Wall -Wextra -pedantic -g
```

> No se usa `-Werror` para evitar fallos por *warnings* menores en otros entornos.

---

## 🧱 Compilación

### Opción A) Comandos directos (rápido)
Desde la **raíz del repo** (donde está la carpeta `proyecto/`):

```bash
# Ejecutable principal (app)
g++ -std=c++14 -O2 -Wall -Wextra -pedantic -g \
  proyecto/main.cpp -I"proyecto/libs" -o app

# Ejecutable de validación (validacion)
g++ -std=c++14 -O2 -Wall -Wextra -pedantic -g \
  "proyecto/validate_export_file (1).cpp" -o validacion
```

> Observa las **comillas** en el segundo comando (el archivo tiene espacios y paréntesis en el nombre).

### Opción B) Makefile (recomendado)
Crea un archivo llamado `Makefile` en la raíz del repo con:

```make
CXX=g++
CXXFLAGS=-std=c++14 -O2 -Wall -Wextra -pedantic -g
INCLUDES=-I"proyecto/libs"

# Ejecutables
all: app validacion

app: proyecto/main.cpp
	$(CXX) $(CXXFLAGS) proyecto/main.cpp $(INCLUDES) -o app

validacion: proyecto/validate_export_file\ \(1\).cpp
	$(CXX) $(CXXFLAGS) "proyecto/validate_export_file (1).cpp" -o validacion

clean:
	rm -f app validacion proyecto/*.o
```

Compila con:
```bash
make        # genera app y validacion
make clean  # limpia ejecutables
```

---

## ▶️ Ejecución

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
> Las rutas de entrada están codificadas en el ejemplo para `data/...`. Puedes cambiarlas en el código si necesitas otro archivo `.bsf` (por ejemplo, el `small` o el `tiny`).

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

## 🔍 ¿Qué hace cada parte? (resumen funcional)
- **Carga de configuración** (`cargarUmbrales`) → lee `configuracion.txt`, parsea con `parseLineaConfiguracion`, almacena en un arreglo dinámico de `Umbral`.
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

## 🧠 Supuestos y validaciones
- `leerArchivoBSF` valida que `numLecturas` sea razonable (p. ej., corta si > 10000).
- Todos los **arreglos son dinámicos**; se libera memoria con `liberarSala` y `delete[]` cuando corresponde.
- Se usan `std::string` **solo** para parseo de líneas de texto; los datos persistentes van en **`char[]`**.

---

## 🧪 Pruebas sugeridas (PC y Raspberry Pi)
1. **Cargar configuración y pacientes** (opción 1) → verificar conteos.
2. **Cargar BSF** (opción 2) → usar `data/patient_readings_simulation 1.bsf` o los archivos `small/tiny`.
3. **Generar anomalias** (opción 3) → revisar `anomalias.txt` (formato legible para médico).
4. **Exportar ECG anómalo** (opción 4) → revisar `pacientes_ecg_anomalos.dat` y convertir con `./validacion`.

---

## 🚀 Publicación en GitHub (pasos rápidos)
> Repositorio: **ProyectoFinal_C++** (público). La carpeta `proyecto/` debe estar en la raíz del repo.

```bash
# 1) Crear repo local
mkdir ProyectoFinal_C++ && cd ProyectoFinal_C++
# Copia aquí tu carpeta "proyecto/" (minúscula)

git init
git add .
git commit -m "Entrega final: proyecto biomédico"

# 2) Crear repositorio vacío en GitHub (vía web) con el mismo nombre
# 3) Conectar y subir
git branch -M main
git remote add origin https://github.com/<tu-usuario>/ProyectoFinal_C++.git
git push -u origin main
```

> Si necesitas renombrar `Proyecto/`→`proyecto/`, hazlo **antes** del `git add .`.

---

## 📄 Licencia
**Sin licencia** (derechos reservados por los autores a efectos académicos).

---

## 👥 Créditos
- **Autores:** Santiago Blanco – Paula Sofía Niño  
- **Curso:** Programación avanzada

---

## ❓ Troubleshooting
- **No compila**: asegúrate de compilar con `-I"proyecto/libs"` y de usar comillas en `"validate_export_file (1).cpp"`.
- **No encuentra archivos**: ejecuta desde la raíz del repo (`./app`), las rutas relativas esperan `proyecto/data/...`.
- **`pacientes_ecg_anomalos.dat` vacío**: puede que el archivo `.bsf` no tenga pacientes con ECG anómalo para el umbral configurado.
- **Raspberry Pi**: usa los mismos comandos; si el equipo es lento, evita `-O0` y mantén `-O2`.

