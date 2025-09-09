cd ~/Documents/PRAV/proyectoFinal_CPP

cat > Makefile <<'EOF'
CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Wextra -pedantic -g
INCLUDES = -I"proyecto/libs"

all: app validacion

app: 
	$(CXX) $(CXXFLAGS) proyecto/main.cpp $(INCLUDES) -o app

<<<<<<< HEAD
# Ejecutable auxiliar de validación
# (sin dependencias con espacios; compilamos en la receta)
validacion:
	$(CXX) $(CXXFLAGS) "proyecto/validate_export_file (1).cpp" -o validacion

# Atajos para ejecutar desde el directorio correcto
=======
validacion:
	$(CXX) $(CXXFLAGS) "proyecto/validate_export_file (1).cpp" -o validacion

>>>>>>> c11e601 (WIP: cambios locales)
run:
	cd proyecto && ../app

validate:
	cd proyecto && ../validacion

clean:
	rm -f app validacion proyecto/*.o

.PHONY: all app validacion run validate clean
<<<<<<< HEAD
EOF
=======
>>>>>>> c11e601 (WIP: cambios locales)
