struct information {
   int number;
   char letter;
   float f;
};

int is_big_endian(){
	int num = 1;
	return (*(char *)&num != 1);
}

float convert_float(float value)  {
	float endian_value= value;  
	char* oldValptr = (char *)&value;  
	char* endianValptr = (char *)&endian_value;  
	if (is_big_endian()) {
		return value;
	}
	endianValptr [0] = oldValptr [3];  
	endianValptr [1] = oldValptr [2]; 
	endianValptr [2] = oldValptr [1];  
	endianValptr [3] = oldValptr [0];  
	return endian_value;  
} 


