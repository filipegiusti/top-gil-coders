	START	inicio
	STACK	20
	EXTDEF	teste
opa	EXTR
inicio	LOAD	c34
	ADD	c2
	STORE	spc
teste	ADD	opa
	STOP
spc	SPACE
c34	CONST	34
c2	CONST	2
	END