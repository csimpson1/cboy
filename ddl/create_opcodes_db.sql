create or replace table operation(
	operation_id int auto_increment primary key,
	mnemonic char(12) not null,
	zero_flag char(5),
	subtract_flag char(5),
	half_carry_flag char(4),
	carry_flag char(4)
);
	
create or replace table operand(
	operand_id int auto_increment primary key,
	name char(3) not null,
	size int not null,
	description char(50) not null
);

create or replace table opcode(
	opcode_id int auto_increment primary key,
	name char(6) not null,
	operation_id int not null,
	bytes int not null,
	cycles int not null,
	conditional_cycles int
);

create or replace table instruction(
	opcode_id int primary key,
	operand_id int not null,
	op_order int not null,
	op_immediate bool not null
);
