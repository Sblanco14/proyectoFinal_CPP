CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Wextra -pedantic -g
INCLUDES = -I"proyecto/libs"

# Construye ambos ejecutables
all: app validacion

# Ejecutable principal
app: proyecto/main.cpp
	$(CXX) $(CXXFLAGS) proyecto/main.cpp $(INCLUDES) -o app

# Ejecutable auxiliar de validación (el archivo tiene espacios y paréntesis)
validacion: proyecto/validate_export_file\ \(1\).cpp
	$(CXX) $(CXXFLAGS) "proyecto/validate_export_file (1).cpp" -o validacion

# Limpieza
clean:
	rm -f app validacion proyecto/*.o

.PHONY: all clean
