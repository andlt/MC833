echo "Qual cliente você quer rodar?"
#read option

for i in `seq 1 30`;do
	echo "Executando client; iteração $i"
	./client 177.220.85.68
done

exit 0
