MAPS=./tests_maps/*
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

function check_valgrind()
{
	echo $1
	${VALGRIND} ./cub3D $1 2>&1 | grep -E "definitely lost|indirectly lost|possibly lost|still reachable|ERROR SUMMARY"
	echo ""
}

rm valgrind_result.txt
for i in ${MAPS}
do
	echo $i
	check_valgrind $i 2>&1 >> valgrind_result.txt
done