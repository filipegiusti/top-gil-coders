	START	inicio
	STACK	20
	EXTDEF	teste
opa	EXTR
inicio	LOAD	c34
	ADD	c2
	ADD	@34
	ADD	@19
	ADD	@H'Ff01'
	ADD	@65534
	STORE	spc
teste	ADD	opa
	STOP
spc	SPACE
c34	CONST	34
c2	CONST	2
c1	CONST	2
c3	CONST	2
c4	CONST	2
c5	CONST	2
c6	CONST	2
c7	CONST	2
	END