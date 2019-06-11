#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libudev.h>

 // Devices are uniquely identified by their syspath.
 // Every device has exactly one path in the kernel's /sys filesystem.
 // Devices usually belong to a kernel subsystem , and have a unique name inside that subsystem.

#define CC_NORM		"\x1B[0m"
#define CC_BRT		"\x1b[1m"
#define CC_BLK		"\x1B[30m"
#define CC_RED		"\x1B[31m"
#define CC_GRN		"\x1B[32m"
#define CC_YEL		"\x1B[33m"
#define CC_BLU		"\x1B[34m"
#define CC_MAG		"\x1B[35m"
#define CC_CYN		"\x1B[36m"
#define CC_WHT		"\x1B[37m"

int main( int argc, char *argv[] ) {

	int restrict_device_properties_listing = 1 ;
	int restrict_device_attributes_listing = 1 ;
	int restrict_device_tag_listing = 0 ;
	int restrict_device_link_listing = 0 ;
	int restrict_device_subsystem_listing = 0 ;
	int restrict_device_type_listing = 0 ;
	int restrict_device_kernel_driver_listing = 0 ;
	int restrict_device_kernel_device_name_listing = 0 ;
	int restrict_device_node_name_listing = 0 ;

	struct udev *pointer_to_struct_udev ; //	struct udev *udev ;
	struct udev_enumerate *pointer_to_struct_udev_enumerate ; //	struct udev_enumerate *enumerate ;
	struct udev_list_entry *pointer_to_struct_udev_list_entry1 , *pointer_to_struct_udev_list_entry2 ; //	struct udev_list_entry *devices , *dev_list_entry ;
	struct udev_device *pointer_to_struct_udev_device ; //	struct udev_device *dev ;
	const char *path ;
	int device_counter = 0 ;
	dev_t device_number ;
	int minor_device_number ;
	int major_device_number ;
	char string_buffer[4096] ;
	char *pointer_to_string_buffer = &string_buffer[0] ;
	char *pointer_to_position_in_buffer ;
	int first_entry = 0 ;
	const char *name , *value ;

	struct udev_list_entry* link_list ;
	struct udev_list_entry* link_list_entry ;
	struct udev_list_entry* tag_list ;
	struct udev_list_entry* tag_list_entry ;
	struct udev_list_entry* attribute_list ;
	struct udev_list_entry* attribute_list_entry ;
	struct udev_list_entry* property_list ;
	struct udev_list_entry* property_list_entry ;

 // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int color_switch = 0 ;
	int counter = 0 ;

	char valid_ascii_characters[ 128 ] = {							// for screening  out non-printable characters from output.
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 ,	// 0 - 15
		0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 ,		// 16 - 31
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,				// 32 - 47
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,				// 48 - 63
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,				// 64 - 79
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,				// 80 - 95
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,				// 96 - 111
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 } ;				// 112 - 127

	struct device_property_strings {
		char property[ 1024 ] ;
		struct device_property_strings *pointer_to_next_node ;
	} ;
	struct device_property_strings *pointer_to_device_properties_root_node = NULL ;
	struct device_property_strings *pointer_to_current_device_properties_node = NULL ;
	struct device_property_strings *pointer_to_next_device_properties_node = NULL ;
	int device_property_found = 0 ;

	struct device_arrtibute_strings {
		char property[ 1024 ] ;
		struct device_arrtibute_strings *pointer_to_next_node ;
	} ;
	struct device_arrtibute_strings *pointer_to_device_attributes_root_node = NULL ;
	struct device_arrtibute_strings *pointer_to_current_device_attributes_node = NULL ;
	struct device_arrtibute_strings *pointer_to_next_device_attributes_node = NULL ;
	int device_attribute_found = 0 ;

	struct device_tag_strings {
		char property[ 1024 ] ;
		struct device_tag_strings *pointer_to_next_node ;
	} ;
	struct device_tag_strings *pointer_to_device_tag_root_node = NULL ;
	struct device_tag_strings *pointer_to_current_device_tag_node = NULL ;
	struct device_tag_strings *pointer_to_next_device_tag_node = NULL ;
	int device_tag_found = 0 ;

	struct device_link_strings {
		char property[ 1024 ] ;
		struct device_link_strings *pointer_to_next_node ;
	} ;
	struct device_link_strings *pointer_to_device_link_root_node = NULL ;
	struct device_link_strings *pointer_to_current_device_link_node = NULL ;
	struct device_link_strings *pointer_to_next_device_link_node = NULL ;
	int device_link_found = 0 ;

	struct device_subsystem_strings {
		char property[ 1024 ] ;
		struct device_subsystem_strings *pointer_to_next_node ;
	} ;
	struct device_subsystem_strings *pointer_to_device_subsystem_root_node = NULL ;
	struct device_subsystem_strings *pointer_to_current_device_subsystem_node = NULL ;
	struct device_subsystem_strings *pointer_to_next_device_subsystem_node = NULL ;
	int device_subsystem_found = 0 ;

	struct device_type_strings {
		char property[ 1024 ] ;
		struct device_type_strings *pointer_to_next_node ;
	} ;
	struct device_type_strings *pointer_to_device_type_root_node = NULL ;
	struct device_type_strings *pointer_to_current_device_type_node = NULL ;
	struct device_type_strings *pointer_to_next_device_type_node = NULL ;
	int device_type_found = 0 ;

	struct device_kernel_driver_strings {
		char property[ 1024 ] ;
		struct device_kernel_driver_strings *pointer_to_next_node ;
	} ;
	struct device_kernel_driver_strings *pointer_to_device_kernel_driver_root_node = NULL ;
	struct device_kernel_driver_strings *pointer_to_current_device_kernel_driver_node = NULL ;
	struct device_kernel_driver_strings *pointer_to_next_device_kernel_driver_node = NULL ;
	int device_kernel_driver_found = 0 ;

	struct device_kernel_device_name_strings {
		char property[ 1024 ] ;
		struct device_kernel_device_name_strings *pointer_to_next_node ;
	} ;
	struct device_kernel_device_name_strings *pointer_to_device_kernel_device_name_root_node = NULL ;
	struct device_kernel_device_name_strings *pointer_to_current_device_kernel_device_name_node = NULL ;
	struct device_kernel_device_name_strings *pointer_to_next_device_kernel_device_name_node = NULL ;
	int device_kernel_device_name_found = 0 ;

	struct device_node_name_strings {
		char property[ 1024 ] ;
		struct device_node_name_strings *pointer_to_next_node ;
	} ;
	struct device_node_name_strings *pointer_to_device_node_name_root_node = NULL ;
	struct device_node_name_strings *pointer_to_current_device_node_name_node = NULL ;
	struct device_node_name_strings *pointer_to_next_device_node_name_node = NULL ;
	int device_node_name_found = 0 ;

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CREATE A DEVICE CONTEXT:
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	pointer_to_struct_udev = udev_new() ;
		if( !pointer_to_struct_udev ){
			fprintf( stderr , "Can't create udev\n" ) ;
			exit( 1 ) ;
		}

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// CREATE A ENUMERATE CONTEXT:
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	pointer_to_struct_udev_enumerate = udev_enumerate_new( pointer_to_struct_udev ) ;

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// FILTERS THAT DETERMINE WHAT THE LIST(enumeration)  RETURNS:
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//udev_enumerate_add_match_subsystem( pointer_to_struct_udev_enumerate , "see-rap" ) ;
	//udev_enumerate_add_match_subsystem( pointer_to_struct_udev_enumerate , "input" ) ;
	//udev_enumerate_add_nomatch_subsystem( pointer_to_struct_udev_enumerate , " input , ) ;
	//udev_enumerate_add_match_sysattr( pointer_to_struct_udev_enumerate , attribute , "value" ) ;
	//udev_enumerate_add_nomatch_sysattr( pointer_to_struct_udev_enumerate , attribute , "value" ) ;
	//udev_enumerate_add_match_property ( pointer_to_struct_udev_enumerate , "ID_INPUT_MOUSE" , "1" ) ;
	//udev_enumerate_add_match_tag( pointer_to_struct_udev_enumerate , "seat" ) ;
	//udev_enumerate_add_match_parent( pointer_to_struct_udev_enumerate , *parent ) ;
	//udev_enumerate_add_match_is_initialized( pointer_to_struct_udev_enumerate ) ;
	//udev_enumerate_add_match_sysname( pointer_to_struct_udev_enumerate , *sysname ) ;
	//udev_enumerate_add_syspath( pointer_to_struct_udev_enumerate , *syspath ) ;
	//udev_enumerate_get_udev( pointer_to_struct_udev_enumerate ) ;

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	SCAN FOR THE DEVICES:
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	udev_enumerate_scan_devices( pointer_to_struct_udev_enumerate ) ; // Look through devices , and create a list.
	//udev_enumerate_scan_subsystems( pointer_to_struct_udev_enumerate ) ;

	pointer_to_struct_udev_list_entry1 = udev_enumerate_get_list_entry( pointer_to_struct_udev_enumerate ) ; // Get the FIRST entry of the sorted list of device paths.


	udev_list_entry_foreach( pointer_to_struct_udev_list_entry2 , pointer_to_struct_udev_list_entry1 ){ //	udev_list_entry_foreach( dev_list_entry , devices ){
	path = udev_list_entry_get_name( pointer_to_struct_udev_list_entry2 ) ;
	pointer_to_struct_udev_device = udev_device_new_from_syspath( pointer_to_struct_udev , path ) ;

	fprintf( stderr , "------------------------------------------------------\n" ) ;
	fprintf( stderr , " %s%s%-7s%s(%s%s%04d%s)%39s%s%s:%s %s%s%s%s\n" ,
		CC_BRT , CC_CYN , "DEVICE:" , CC_NORM ,
		CC_BRT , CC_BLU , device_counter , CC_NORM , "" , CC_BRT , CC_WHT , CC_NORM ,
		CC_BRT , CC_BLU , udev_device_get_syspath( pointer_to_struct_udev_device ) , CC_NORM ) ;

	fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Device path" , CC_BLU , udev_device_get_devpath( pointer_to_struct_udev_device ) , CC_NORM ) ;
	fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Instance" , CC_BLU , udev_device_get_sysnum( pointer_to_struct_udev_device ) , CC_NORM ) ;
	device_number = udev_device_get_devnum( pointer_to_struct_udev_device ) ;
	major_device_number = ( device_number >> 8 ) ;
	minor_device_number = ( device_number & 255 ) ;
	fprintf( stderr , " %s%s%-52s: %s%d:%s%d%s\n" , CC_BRT , CC_WHT , "Major/Minor device number" ,
			CC_BLU , major_device_number , CC_BLU , minor_device_number , CC_NORM ) ;

	name = udev_device_get_subsystem( pointer_to_struct_udev_device ) ;
	fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Subsytem" , CC_BLU , name , CC_NORM ) ;
	if( name != NULL ){
		if( pointer_to_device_subsystem_root_node == NULL ){
		pointer_to_device_subsystem_root_node = ( struct device_subsystem_strings * )malloc( sizeof( struct device_subsystem_strings ) ) ;
			if( pointer_to_device_subsystem_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
		strcpy( pointer_to_device_subsystem_root_node->property , name ) ;
		pointer_to_device_subsystem_root_node->pointer_to_next_node = NULL ;
		} else {
		pointer_to_current_device_subsystem_node = pointer_to_device_subsystem_root_node ;
		device_subsystem_found = 0 ;
			while( pointer_to_current_device_subsystem_node->pointer_to_next_node != NULL ){
				if( strcmp( name , pointer_to_current_device_subsystem_node->property ) == 0 ){
					device_subsystem_found = 1 ;
					break ;
				}
				pointer_to_current_device_subsystem_node = pointer_to_current_device_subsystem_node->pointer_to_next_node ;
			}
			if( device_subsystem_found == 0 ){
				if( strcmp( name , pointer_to_current_device_subsystem_node->property ) == 0 ){
				} else {
					pointer_to_next_device_subsystem_node = ( struct device_subsystem_strings * )malloc( sizeof( struct device_subsystem_strings ) ) ;
						if( pointer_to_next_device_subsystem_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
					pointer_to_next_device_subsystem_node->pointer_to_next_node = NULL ;
					strcpy( pointer_to_next_device_subsystem_node->property , name ) ;
					pointer_to_current_device_subsystem_node->pointer_to_next_node = pointer_to_next_device_subsystem_node ;
				}
			}
		}
	}

	name = udev_device_get_devnode( pointer_to_struct_udev_device ) ;
	fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Device node" , CC_BLU , name , CC_NORM ) ;
	if( name != NULL ){
		if( pointer_to_device_node_name_root_node == NULL ){
		pointer_to_device_node_name_root_node = ( struct device_node_name_strings * )malloc( sizeof( struct device_node_name_strings ) ) ;
			if( pointer_to_device_node_name_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
		strcpy( pointer_to_device_node_name_root_node->property , name ) ;
		pointer_to_device_node_name_root_node->pointer_to_next_node = NULL ;
		} else {
		pointer_to_current_device_node_name_node = pointer_to_device_node_name_root_node ;
		device_node_name_found = 0 ;
			while( pointer_to_current_device_node_name_node->pointer_to_next_node != NULL ){
				if( strcmp( name , pointer_to_current_device_node_name_node->property ) == 0 ){
					device_node_name_found = 1 ;
					break ;
				}
				pointer_to_current_device_node_name_node = pointer_to_current_device_node_name_node->pointer_to_next_node ;
			}
			if( device_node_name_found == 0 ){
				if( strcmp( name , pointer_to_current_device_node_name_node->property ) == 0 ){
				} else {
					pointer_to_next_device_node_name_node = ( struct device_node_name_strings * )malloc( sizeof( struct device_node_name_strings ) ) ;
						if( pointer_to_next_device_node_name_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
					pointer_to_next_device_node_name_node->pointer_to_next_node = NULL ;
					strcpy( pointer_to_next_device_node_name_node->property , name ) ;
					pointer_to_current_device_node_name_node->pointer_to_next_node = pointer_to_next_device_node_name_node ;
				}
			}
		}
	}

	name = udev_device_get_sysname( pointer_to_struct_udev_device ) ;
	fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Kernel device name" , CC_BLU , name , CC_NORM ) ;
	if( name != NULL ){
		if( pointer_to_device_kernel_device_name_root_node == NULL ){
		pointer_to_device_kernel_device_name_root_node = ( struct device_kernel_device_name_strings * )malloc( sizeof( struct device_kernel_device_name_strings ) ) ;
			if( pointer_to_device_kernel_device_name_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
		strcpy( pointer_to_device_kernel_device_name_root_node->property , name ) ;
		pointer_to_device_kernel_device_name_root_node->pointer_to_next_node = NULL ;
		} else {
		pointer_to_current_device_kernel_device_name_node = pointer_to_device_kernel_device_name_root_node ;
		device_kernel_device_name_found = 0 ;
			while( pointer_to_current_device_kernel_device_name_node->pointer_to_next_node != NULL ){
				if( strcmp( name , pointer_to_current_device_kernel_device_name_node->property ) == 0 ){
					device_kernel_device_name_found = 1 ;
					break ;
				}
				pointer_to_current_device_kernel_device_name_node = pointer_to_current_device_kernel_device_name_node->pointer_to_next_node ;
			}
			if( device_kernel_device_name_found == 0 ){
				if( strcmp( name , pointer_to_current_device_kernel_device_name_node->property ) == 0 ){
				} else {
					pointer_to_next_device_kernel_device_name_node = ( struct device_kernel_device_name_strings * )malloc( sizeof( struct device_kernel_device_name_strings ) ) ;
						if( pointer_to_next_device_kernel_device_name_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
					pointer_to_next_device_kernel_device_name_node->pointer_to_next_node = NULL ;
					strcpy( pointer_to_next_device_kernel_device_name_node->property , name ) ;
					pointer_to_current_device_kernel_device_name_node->pointer_to_next_node = pointer_to_next_device_kernel_device_name_node ;
				}
			}
		}
	}

	name = udev_device_get_driver( pointer_to_struct_udev_device ) ;
	fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Kernel driver name" , CC_BLU , name , CC_NORM ) ;
	if( name != NULL ){
		if( pointer_to_device_kernel_driver_root_node == NULL ){
		pointer_to_device_kernel_driver_root_node = ( struct device_kernel_driver_strings * )malloc( sizeof( struct device_kernel_driver_strings ) ) ;
			if( pointer_to_device_kernel_driver_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
		strcpy( pointer_to_device_kernel_driver_root_node->property , name ) ;
		pointer_to_device_kernel_driver_root_node->pointer_to_next_node = NULL ;
		} else {
		pointer_to_current_device_kernel_driver_node = pointer_to_device_kernel_driver_root_node ;
		device_kernel_driver_found = 0 ;
			while( pointer_to_current_device_kernel_driver_node->pointer_to_next_node != NULL ){
				if( strcmp( name , pointer_to_current_device_kernel_driver_node->property ) == 0 ){
					device_kernel_driver_found = 1 ;
					break ;
				}
				pointer_to_current_device_kernel_driver_node = pointer_to_current_device_kernel_driver_node->pointer_to_next_node ;
			}
			if( device_kernel_driver_found == 0 ){
				if( strcmp( name , pointer_to_current_device_kernel_driver_node->property ) == 0 ){
				} else {
					pointer_to_next_device_kernel_driver_node = ( struct device_kernel_driver_strings * )malloc( sizeof( struct device_kernel_driver_strings ) ) ;
						if( pointer_to_next_device_kernel_driver_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
					pointer_to_next_device_kernel_driver_node->pointer_to_next_node = NULL ;
					strcpy( pointer_to_next_device_kernel_driver_node->property , name ) ;
					pointer_to_current_device_kernel_driver_node->pointer_to_next_node = pointer_to_next_device_kernel_driver_node ;
				}
			}
		}
	}

	name = udev_device_get_devtype( pointer_to_struct_udev_device ) ;
	fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Device type" , CC_BLU , name , CC_NORM ) ;
	if( name != NULL ){
		if( pointer_to_device_type_root_node == NULL ){
		pointer_to_device_type_root_node = ( struct device_type_strings * )malloc( sizeof( struct device_type_strings ) ) ;
			if( pointer_to_device_type_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
		strcpy( pointer_to_device_type_root_node->property , name ) ;
		pointer_to_device_type_root_node->pointer_to_next_node = NULL ;
		} else {
		pointer_to_current_device_type_node = pointer_to_device_type_root_node ;
		device_type_found = 0 ;
			while( pointer_to_current_device_type_node->pointer_to_next_node != NULL ){
				if( strcmp( name , pointer_to_current_device_type_node->property ) == 0 ){
					device_type_found = 1 ;
					break ;
				}
				pointer_to_current_device_type_node = pointer_to_current_device_type_node->pointer_to_next_node ;
			}
			if( device_type_found == 0 ){
				if( strcmp( name , pointer_to_current_device_type_node->property ) == 0 ){
				} else {
					pointer_to_next_device_type_node = ( struct device_type_strings * )malloc( sizeof( struct device_type_strings ) ) ;
						if( pointer_to_next_device_type_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
					pointer_to_next_device_type_node->pointer_to_next_node = NULL ;
					strcpy( pointer_to_next_device_type_node->property , name ) ;
					pointer_to_current_device_type_node->pointer_to_next_node = pointer_to_next_device_type_node ;
				}
			}
		}
	}

	if( udev_device_get_is_initialized( pointer_to_struct_udev_device ) == 0 ){
		fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Device Initialized" , CC_RED , "FALSE" , CC_NORM ) ;
	} else {
		fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , "Device Initialized" , CC_GRN , "TRUE" , CC_NORM ) ;
	}

		property_list = udev_device_get_properties_list_entry( pointer_to_struct_udev_device ) ;
		if( property_list != NULL ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT , CC_CYN , "DEVICE PROPERTIES:" , CC_NORM ) ;
			udev_list_entry_foreach( property_list_entry , property_list ){
				name = udev_list_entry_get_name( property_list_entry ) ;
				value = udev_list_entry_get_value( property_list_entry ) ;
				fprintf( stderr , " %s%s%-52s: %s%s%s\n" , CC_BRT , CC_WHT , name , CC_GRN , value , CC_NORM ) ;
				if( pointer_to_device_properties_root_node == NULL ){
				pointer_to_device_properties_root_node = ( struct device_property_strings * )malloc( sizeof( struct device_property_strings ) ) ;
					if( pointer_to_device_properties_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
				strcpy( pointer_to_device_properties_root_node->property , name ) ;
				pointer_to_device_properties_root_node->pointer_to_next_node = NULL ;
				} else {
				pointer_to_current_device_properties_node = pointer_to_device_properties_root_node ;
				device_property_found = 0 ;
					while( pointer_to_current_device_properties_node->pointer_to_next_node != NULL ){
						if( strcmp( name , pointer_to_current_device_properties_node->property ) == 0 ){
							device_property_found = 1 ;
							break ;
						}
						pointer_to_current_device_properties_node = pointer_to_current_device_properties_node->pointer_to_next_node ;
					}
					if( device_property_found == 0 ){
						if( strcmp( name , pointer_to_current_device_properties_node->property ) == 0 ){
							break ;
						} else {
							pointer_to_next_device_properties_node = ( struct device_property_strings * )malloc( sizeof( struct device_property_strings ) ) ;
								if( pointer_to_next_device_properties_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
							pointer_to_next_device_properties_node->pointer_to_next_node = NULL ;
							strcpy( pointer_to_next_device_properties_node->property , name ) ;
							pointer_to_current_device_properties_node->pointer_to_next_node = pointer_to_next_device_properties_node ;
						}
					}
				}
			}
		}

		attribute_list = udev_device_get_sysattr_list_entry( pointer_to_struct_udev_device ) ;
		if( attribute_list != NULL ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT , CC_CYN , "DEVICE ATTRIBUTES:" , CC_NORM ) ;
			udev_list_entry_foreach( attribute_list_entry , attribute_list ){
				name = udev_list_entry_get_name( attribute_list_entry ) ;
				value = udev_device_get_sysattr_value( pointer_to_struct_udev_device , name ) ;
				if( value != NULL ){
					strcpy( string_buffer , value ) ;
					pointer_to_string_buffer = &string_buffer[0] ;
					while( ( pointer_to_position_in_buffer = strsep( &pointer_to_string_buffer , "\n" ) ) != NULL ){
						if( first_entry == 0 ){
							fprintf( stderr , " %s%s%-52s: %s" , CC_BRT , CC_WHT , name , CC_NORM ) ;
							counter = 0 ;
							while( pointer_to_position_in_buffer[ counter ] != '\0' ) {
								if( valid_ascii_characters[ (int)pointer_to_position_in_buffer[ counter ] ] == 1 ){
									fprintf( stderr , "%s%s%c%s" , CC_BRT , CC_RED , pointer_to_position_in_buffer[ counter ] , CC_NORM ) ;
								} else {
									fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , "?" , CC_NORM ) ;
								}
								counter++ ;
							}
							fprintf( stderr , "\n" ) ;
							first_entry = 1 ;
						} else {
							fprintf( stderr , " %s%s%-52s%s%s: %s" , CC_BRT , CC_BLK , name , CC_BRT , CC_WHT ,CC_NORM ) ;
							counter = 0 ;
							while( pointer_to_position_in_buffer[ counter ] != '\0' ) {
								if( valid_ascii_characters[ (int)pointer_to_position_in_buffer[ counter ] ] == 1 ){
									fprintf( stderr , "%s%s%c%s" , CC_BRT , CC_RED , pointer_to_position_in_buffer[ counter ] , CC_NORM ) ;
								} else {
									fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , "?" , CC_NORM ) ;
								}
								counter++ ;
							}
							fprintf( stderr , "\n" ) ;
						}
					}
				}

				if( pointer_to_device_attributes_root_node == NULL ){
				pointer_to_device_attributes_root_node = ( struct device_arrtibute_strings * )malloc( sizeof( struct device_arrtibute_strings ) ) ;
					if( pointer_to_device_attributes_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
				strcpy( pointer_to_device_attributes_root_node->property , name ) ;
				pointer_to_device_attributes_root_node->pointer_to_next_node = NULL ;
				} else {
				pointer_to_current_device_attributes_node = pointer_to_device_attributes_root_node ;
				device_attribute_found = 0 ;
				first_entry = 0 ;
					while( pointer_to_current_device_attributes_node->pointer_to_next_node != NULL ){
						if( strcmp( name , pointer_to_current_device_attributes_node->property ) == 0 ){
							device_attribute_found = 1 ;
							break ;
						}
						pointer_to_current_device_attributes_node = pointer_to_current_device_attributes_node->pointer_to_next_node ;
					}
					if( device_attribute_found == 0 ){
						if( strcmp( name , pointer_to_current_device_attributes_node->property ) == 0 ){
							break ;
						} else {
							pointer_to_next_device_attributes_node = ( struct device_arrtibute_strings * )malloc( sizeof( struct device_arrtibute_strings ) ) ;
								if( pointer_to_next_device_attributes_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
							pointer_to_next_device_attributes_node->pointer_to_next_node = NULL ;
							strcpy( pointer_to_next_device_attributes_node->property , name ) ;
							pointer_to_current_device_attributes_node->pointer_to_next_node = pointer_to_next_device_attributes_node ;
						}
					}
				}
			}
		}

		tag_list = udev_device_get_tags_list_entry( pointer_to_struct_udev_device ) ;
		if( tag_list != NULL ){
			fprintf( stderr , " %s%s%-52s%s\n" , CC_BRT , CC_CYN , "DEVICE TAGS:" , CC_NORM ) ;
			udev_list_entry_foreach( tag_list_entry , tag_list ){
				name = udev_list_entry_get_name( tag_list_entry ) ;
				fprintf( stderr , " %-52s%s%s:%s %s%s%s%s\n" , "" , CC_BRT , CC_WHT , CC_NORM , CC_BRT , CC_MAG , name , CC_NORM ) ;
				if( pointer_to_device_tag_root_node == NULL ){
				pointer_to_device_tag_root_node = ( struct device_tag_strings * )malloc( sizeof( struct device_tag_strings ) ) ;
					if( pointer_to_device_tag_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
				strcpy( pointer_to_device_tag_root_node->property , name ) ;
				pointer_to_device_tag_root_node->pointer_to_next_node = NULL ;
				} else {
				pointer_to_current_device_tag_node = pointer_to_device_tag_root_node ;
				device_tag_found = 0 ;
				first_entry = 0 ;
					while( pointer_to_current_device_tag_node->pointer_to_next_node != NULL ){
						if( strcmp( name , pointer_to_current_device_tag_node->property ) == 0 ){
							device_tag_found = 1 ;
							break ;
						}
						pointer_to_current_device_tag_node = pointer_to_current_device_tag_node->pointer_to_next_node ;
					}
					if( device_tag_found == 0 ){
						if( strcmp( name , pointer_to_current_device_tag_node->property ) == 0 ){
							break ;
						} else {
							pointer_to_next_device_tag_node = ( struct device_tag_strings * )malloc( sizeof( struct device_tag_strings ) ) ;
								if( pointer_to_next_device_tag_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
							pointer_to_next_device_tag_node->pointer_to_next_node = NULL ;
							strcpy( pointer_to_next_device_tag_node->property , name ) ;
							pointer_to_current_device_tag_node->pointer_to_next_node = pointer_to_next_device_tag_node ;
						}
					}
				}
			}
		}

		link_list = udev_device_get_devlinks_list_entry( pointer_to_struct_udev_device ) ;
		if( link_list != NULL ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT , CC_CYN , "DEVICE LINKS:" , CC_NORM ) ;
			udev_list_entry_foreach( link_list_entry , link_list ){
				name = udev_list_entry_get_name( link_list_entry ) ;
				fprintf( stderr , " %-52s%s%s:%s %s%s%s\n" , "" , CC_BRT , CC_WHT , CC_NORM , CC_CYN , name , CC_NORM ) ;
				if( pointer_to_device_link_root_node == NULL ){
				pointer_to_device_link_root_node = ( struct device_link_strings * )malloc( sizeof( struct device_link_strings ) ) ;
					if( pointer_to_device_link_root_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
				strcpy( pointer_to_device_link_root_node->property , name ) ;
				pointer_to_device_link_root_node->pointer_to_next_node = NULL ;
				} else {
				pointer_to_current_device_link_node = pointer_to_device_link_root_node ;
				device_link_found = 0 ;
					while( pointer_to_current_device_link_node->pointer_to_next_node != NULL ){
						if( strcmp( name , pointer_to_current_device_link_node->property ) == 0 ){
							device_link_found = 1 ;
							break ;
						}
						pointer_to_current_device_link_node = pointer_to_current_device_link_node->pointer_to_next_node ;
					}
					if( device_link_found == 0 ){
						if( strcmp( name , pointer_to_current_device_link_node->property ) == 0 ){
							break ;
						} else {
							pointer_to_next_device_link_node = ( struct device_link_strings * )malloc( sizeof( struct device_link_strings ) ) ;
								if( pointer_to_next_device_link_node == NULL ){ fprintf( stderr , "%s%s%s%s" , CC_BRT , CC_RED , " ERROR: malloc returned null Exiting..." , CC_NORM ) ; exit( 0 ) ; }
							pointer_to_next_device_link_node->pointer_to_next_node = NULL ;
							strcpy( pointer_to_next_device_link_node->property , name ) ;
							pointer_to_current_device_link_node->pointer_to_next_node = pointer_to_next_device_link_node ;
						}
					}
				}
			}
		}

		udev_device_unref( pointer_to_struct_udev_device ) ;
		device_counter +=1 ;

	}

	if( restrict_device_properties_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL DEVICE PROPERTIES FOUND:" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_properties_root_node != NULL ){
		pointer_to_current_device_properties_node = pointer_to_device_properties_root_node ;
		while( pointer_to_current_device_properties_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_GRN , pointer_to_current_device_properties_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s\n" ,  CC_GRN , pointer_to_current_device_properties_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_properties_node = pointer_to_current_device_properties_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_GRN , pointer_to_current_device_properties_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s\n" ,  CC_GRN , pointer_to_current_device_properties_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No device properties found." , CC_NORM ) ;
		}
	}

	if( restrict_device_attributes_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL DEVICE ATTRIBUTES FOUND:" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_attributes_root_node != NULL ){
		pointer_to_current_device_attributes_node = pointer_to_device_attributes_root_node ;
		while( pointer_to_current_device_attributes_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_RED , pointer_to_current_device_attributes_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s\n" ,  CC_RED , pointer_to_current_device_attributes_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_attributes_node = pointer_to_current_device_attributes_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_RED , pointer_to_current_device_attributes_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s\n" ,  CC_RED , pointer_to_current_device_attributes_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No device attributes found." , CC_NORM ) ;
		}
	}

	if( restrict_device_tag_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL DEVICE TAGS FOUND:" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_tag_root_node != NULL ){
		pointer_to_current_device_tag_node = pointer_to_device_tag_root_node ;
		while( pointer_to_current_device_tag_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_MAG , pointer_to_current_device_tag_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s\n" ,  CC_MAG , pointer_to_current_device_tag_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_tag_node = pointer_to_current_device_tag_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_MAG , pointer_to_current_device_tag_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s\n" ,  CC_MAG , pointer_to_current_device_tag_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No device tags found." , CC_NORM ) ;
		}
	}

	if( restrict_device_link_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL DEVICE LINKS FOUND:" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_link_root_node != NULL ){
		pointer_to_current_device_link_node = pointer_to_device_link_root_node ;
		while( pointer_to_current_device_link_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_BLU , pointer_to_current_device_link_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , pointer_to_current_device_link_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_link_node = pointer_to_current_device_link_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_BLU , pointer_to_current_device_link_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_BRT , CC_CYN , pointer_to_current_device_link_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No device links found." , CC_NORM ) ;
		}
	}

	if( restrict_device_subsystem_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL DEVICE SUBSYTEMS FOUND:" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_subsystem_root_node != NULL ){
		pointer_to_current_device_subsystem_node = pointer_to_device_subsystem_root_node ;
		while( pointer_to_current_device_subsystem_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_YEL , pointer_to_current_device_subsystem_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" , CC_NORM ,  CC_YEL , pointer_to_current_device_subsystem_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_subsystem_node = pointer_to_current_device_subsystem_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_YEL , pointer_to_current_device_subsystem_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_YEL , pointer_to_current_device_subsystem_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No subsytem entries found." , CC_NORM ) ;
		}
	}

	if( restrict_device_type_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL DEVICE TYPES FOUND:" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;
	if( pointer_to_device_type_root_node != NULL ){
		pointer_to_current_device_type_node = pointer_to_device_type_root_node ;
		while( pointer_to_current_device_type_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_WHT , pointer_to_current_device_type_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" , CC_NORM ,  CC_WHT , pointer_to_current_device_type_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_type_node = pointer_to_current_device_type_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_WHT , pointer_to_current_device_type_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_WHT , pointer_to_current_device_type_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No type entries found." , CC_NORM ) ;
		}
	}

	if( restrict_device_kernel_driver_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL KERNEL DRIVERS FOUND:" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_kernel_driver_root_node != NULL ){
		pointer_to_current_device_kernel_driver_node = pointer_to_device_kernel_driver_root_node ;
		while( pointer_to_current_device_kernel_driver_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_GRN , pointer_to_current_device_kernel_driver_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" , CC_NORM ,  CC_GRN , pointer_to_current_device_kernel_driver_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_kernel_driver_node = pointer_to_current_device_kernel_driver_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_GRN , pointer_to_current_device_kernel_driver_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_GRN , pointer_to_current_device_kernel_driver_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No kernel driver entries found." , CC_NORM ) ;
		}
	}

	if( restrict_device_kernel_device_name_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL KERNEL DEVICE NAMES :" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_kernel_device_name_root_node != NULL ){
		pointer_to_current_device_kernel_device_name_node = pointer_to_device_kernel_device_name_root_node ;
		while( pointer_to_current_device_kernel_device_name_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_RED , pointer_to_current_device_kernel_device_name_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" , CC_NORM ,  CC_RED , pointer_to_current_device_kernel_device_name_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_kernel_device_name_node = pointer_to_current_device_kernel_device_name_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_RED , pointer_to_current_device_kernel_device_name_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , pointer_to_current_device_kernel_device_name_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No kernel device names found." , CC_NORM ) ;
		}
	}

	if( restrict_device_node_name_listing == 0 ){
		fprintf( stderr , "\n------------------------------------------------------\n" ) ;
		fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_CYN , "ALL DEVICE NODES :" , CC_NORM ) ;
		fprintf( stderr , "------------------------------------------------------\n\n" ) ;

	if( pointer_to_device_node_name_root_node != NULL ){
		pointer_to_current_device_node_name_node = pointer_to_device_node_name_root_node ;
		while( pointer_to_current_device_node_name_node->pointer_to_next_node != NULL ){
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_MAG , pointer_to_current_device_node_name_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" , CC_NORM ,  CC_MAG , pointer_to_current_device_node_name_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
			pointer_to_current_device_node_name_node = pointer_to_current_device_node_name_node->pointer_to_next_node ;
		} ;
			if( color_switch == 0 ){
			fprintf( stderr , " %s%s%s%s\n" , CC_BRT ,  CC_MAG , pointer_to_current_device_node_name_node->property , CC_NORM ) ;
			color_switch = 1 ;
			} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_MAG , pointer_to_current_device_node_name_node->property , CC_NORM ) ;
			color_switch = 0 ;
			}
		} else {
			fprintf( stderr , " %s%s%s%s\n" ,  CC_NORM , CC_RED , "No device node entries found." , CC_NORM ) ;
		}
	}

	fprintf( stderr , "\n------------------------------------------------------\n\n" ) ;
	udev_enumerate_unref( pointer_to_struct_udev_enumerate ) ; 	udev_unref( pointer_to_struct_udev ) ;

	// Free linked list memory.

		if( pointer_to_device_properties_root_node != NULL ){
			pointer_to_current_device_properties_node = pointer_to_device_properties_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_properties_node != NULL ){
				pointer_to_next_device_properties_node = pointer_to_current_device_properties_node->pointer_to_next_node ;
				free( pointer_to_current_device_properties_node ) ;
				pointer_to_current_device_properties_node = pointer_to_next_device_properties_node ;
			}
			free( pointer_to_current_device_properties_node ) ;
		}

		if( pointer_to_device_attributes_root_node != NULL ){
			pointer_to_current_device_attributes_node = pointer_to_device_attributes_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_attributes_node != NULL ){
				pointer_to_next_device_attributes_node = pointer_to_current_device_attributes_node->pointer_to_next_node ;
				free( pointer_to_current_device_attributes_node ) ;
				pointer_to_current_device_attributes_node = pointer_to_next_device_attributes_node ;
			}
			free( pointer_to_current_device_attributes_node ) ;
		}

		if( pointer_to_device_tag_root_node != NULL ){
			pointer_to_current_device_tag_node = pointer_to_device_tag_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_tag_node != NULL ){
				pointer_to_next_device_tag_node = pointer_to_current_device_tag_node->pointer_to_next_node ;
				free( pointer_to_current_device_tag_node ) ;
				pointer_to_current_device_tag_node = pointer_to_next_device_tag_node ;
			}
			free( pointer_to_current_device_tag_node ) ;
		}

		if( pointer_to_device_link_root_node != NULL ){
			pointer_to_current_device_link_node = pointer_to_device_link_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_link_node != NULL ){
				pointer_to_next_device_link_node = pointer_to_current_device_link_node->pointer_to_next_node ;
				free( pointer_to_current_device_link_node ) ;
				pointer_to_current_device_link_node = pointer_to_next_device_link_node ;
			}
			free( pointer_to_current_device_link_node ) ;
		}

		if( pointer_to_device_subsystem_root_node != NULL ){
			pointer_to_current_device_subsystem_node = pointer_to_device_subsystem_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_subsystem_node != NULL ){
				pointer_to_next_device_subsystem_node = pointer_to_current_device_subsystem_node->pointer_to_next_node ;
				free( pointer_to_current_device_subsystem_node ) ;
				pointer_to_current_device_subsystem_node = pointer_to_next_device_subsystem_node ;
			}
			free( pointer_to_current_device_subsystem_node ) ;
		}

		if( pointer_to_device_type_root_node != NULL ){
			pointer_to_current_device_type_node = pointer_to_device_type_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_type_node != NULL ){
				pointer_to_next_device_type_node = pointer_to_current_device_type_node->pointer_to_next_node ;
				free( pointer_to_current_device_type_node ) ;
				pointer_to_current_device_type_node = pointer_to_next_device_type_node ;
			}
			free( pointer_to_current_device_type_node ) ;
		}

		if( pointer_to_device_kernel_driver_root_node != NULL ){
			pointer_to_current_device_kernel_driver_node = pointer_to_device_kernel_driver_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_kernel_driver_node != NULL ){
				pointer_to_next_device_kernel_driver_node = pointer_to_current_device_kernel_driver_node->pointer_to_next_node ;
				free( pointer_to_current_device_kernel_driver_node ) ;
				pointer_to_current_device_kernel_driver_node = pointer_to_next_device_kernel_driver_node ;
			}
			free( pointer_to_current_device_kernel_driver_node ) ;
		}

		if( pointer_to_device_kernel_device_name_root_node != NULL ){
			pointer_to_current_device_kernel_device_name_node = pointer_to_device_kernel_device_name_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_kernel_device_name_node != NULL ){
				pointer_to_next_device_kernel_device_name_node = pointer_to_current_device_kernel_device_name_node->pointer_to_next_node ;
				free( pointer_to_current_device_kernel_device_name_node ) ;
				pointer_to_current_device_kernel_device_name_node = pointer_to_next_device_kernel_device_name_node ;
			}
			free( pointer_to_current_device_kernel_device_name_node ) ;
		}

		if( pointer_to_device_node_name_root_node != NULL ){
			pointer_to_current_device_node_name_node = pointer_to_device_node_name_root_node->pointer_to_next_node ;
			while( pointer_to_current_device_node_name_node != NULL ){
				pointer_to_next_device_node_name_node = pointer_to_current_device_node_name_node->pointer_to_next_node ;
				free( pointer_to_current_device_node_name_node ) ;
				pointer_to_current_device_node_name_node = pointer_to_next_device_node_name_node ;
			}
			free( pointer_to_current_device_node_name_node ) ;
		}

	// Free all my pointers.

	pointer_to_device_properties_root_node = NULL ;
	pointer_to_current_device_properties_node = NULL ;
	pointer_to_next_device_properties_node = NULL ;
	pointer_to_device_attributes_root_node = NULL ;
	pointer_to_current_device_attributes_node = NULL ;
	pointer_to_next_device_attributes_node = NULL ;
	pointer_to_device_tag_root_node = NULL ;
	pointer_to_current_device_tag_node = NULL ;
	pointer_to_next_device_tag_node = NULL ;
	pointer_to_device_link_root_node = NULL ;
	pointer_to_current_device_link_node = NULL ;
	pointer_to_next_device_link_node = NULL ;
	pointer_to_device_subsystem_root_node = NULL ;
	pointer_to_current_device_subsystem_node = NULL ;
	pointer_to_next_device_subsystem_node = NULL ;
	pointer_to_device_type_root_node = NULL ;
	pointer_to_current_device_type_node = NULL ;
	pointer_to_next_device_type_node = NULL ;
	pointer_to_device_kernel_driver_root_node = NULL ;
	pointer_to_current_device_kernel_driver_node = NULL ;
	pointer_to_next_device_kernel_driver_node = NULL ;
	pointer_to_device_kernel_device_name_root_node = NULL ;
	pointer_to_current_device_kernel_device_name_node = NULL ;
	pointer_to_next_device_kernel_device_name_node = NULL ;
	pointer_to_device_node_name_root_node = NULL ;
	pointer_to_current_device_node_name_node = NULL ;
	pointer_to_next_device_node_name_node = NULL ;

	pointer_to_struct_udev = NULL ;
	pointer_to_struct_udev_enumerate = NULL ;
	pointer_to_struct_udev_list_entry1 = NULL ;
	pointer_to_struct_udev_list_entry2 = NULL ;
	pointer_to_struct_udev_device = NULL ;
	path = NULL ;
	pointer_to_string_buffer = NULL ;
	pointer_to_position_in_buffer = NULL ;
	name = NULL ;
	value = NULL ;
	link_list = NULL ;
	link_list_entry = NULL ;
	tag_list = NULL ;
	tag_list_entry = NULL ;
	attribute_list = NULL ;
	attribute_list_entry = NULL ;
	property_list = NULL ;
	property_list_entry = NULL ;




}
