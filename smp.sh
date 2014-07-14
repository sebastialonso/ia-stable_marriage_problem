#!/bin/bash
instancia="instancias"
#instancia="Set_21_234"
#instancia="Set_32_234"
#instancia="Set_33_234"
#instancia="Set_64_234"
#instancia="Set_66_234"
#instancia="Set_100_234"
#instancia="Set_102_234"

#Se hace el supuesto que el ejecutable se encuentra en el mismo nivel de directorio de las instancias (carpetas).
#Nombre ejecutable
ejecutable="./gs"

if [ -f "$instancia"".result" ]; then 
	rm -rf "$instancia"".result"
fi


cd $instancia #se accede a la instacia
for f in `ls -1 *.txt`; do #se lee cada problema
	cd ..  # se retrocede un nivel de directorio para poder ejecutar el codigo.
	echo $f
	OUTPUT=$((time $ejecutable $instancia/$f) 2>&1) #se captura el output, por ejemplo: time ./Top p2.2.a.txt 
	echo "$f" $OUTPUT >> "$instancia"".result" 
        #se guarda el nombre del problema (por eso es sin salto de linea), score y tiempo en el archivo instancia.result
	cd $instancia 
done
cd ..


