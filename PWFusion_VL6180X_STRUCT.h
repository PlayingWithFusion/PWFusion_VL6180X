#ifndef PWF_VL6180X_STRUCT_H
#define PWF_VL6180X_STRUCT_H

struct var_vl6180x{
	uint8_t  IC_found;			// 1 = chip found, 0 = chip not found
	uint8_t  chip_init;			// 1 = init OK, 0 = init not OK
	uint8_t  ID_ModelRevMaj;	// Model Major revision number
	uint8_t  ID_ModelRevMin;	// Model Minor revision number
	uint8_t  ID_ModuleRevMaj;	// Module Major revision number
	uint8_t  ID_ModuleRevMin;	// Module Minor revision number
	uint8_t  ID_year;			// single digit build year... 201X
	uint8_t	 ID_month;			// build month
	uint8_t  ID_day;			// build day
	uint16_t ID_seconds;		// build seconds
	uint8_t  dist_val;			// distance value, mm
	float	 LUX_val;			// ambient light value, LUX
};

#endif /* PWF_VL6180X_STRUCT_H */