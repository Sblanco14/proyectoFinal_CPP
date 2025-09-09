CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Wextra -pedantic -g
INCLUDES = -I"proyecto/libs"

# Construye ambos ejecutables
all: app validacion

# Ejecutable principal
app: proyecto/main.cpp
	$(CXX) $(CXXFLAGS) proyecto/main.cpp $(INCLUDES) -o app

# Ejecutable auxiliar de validación
# Nota: NO listamos el .cpp como dependencia por el tema de espacios/paréntesis.
validacion:
	$(CXX) $(CXXFLAGS) "proyecto/validate_export_file (1).cpp" -o validacion

# Atajos para ejecutar desde el directorio correcto
run:
	cd proyecto && ../app

validate:
	cd proyecto && ../validacion

clean:
	rm -f app validacion proyecto/*.o

.PHONY: all app validacion run validate clean
