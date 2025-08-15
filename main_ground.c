#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cli_ground.h"

extern gs_conf_params gs_conf;
extern char* hostname;
extern FILE* cmd_log;

int main(int argc, char* argv[]){
	char cmd_file[1024];
	if(argc<3){
		printf("Usage: %s <config_file> <host_name>\n", argv[0]);
		return 1;
	}
	
	// Display the beautiful BVEX banner
	print_bvex_banner();
	
	printf("This is BCP on %s\n", argv[2]);
	printf("======================\n");
	hostname = strdup(argv[2]);
	get_config_params(argv[1]);
	print_config();
	
	connect_to_sock();
        
	printf("Connecting to %s and %s .....\n",gs_conf.sag_ip,gs_conf.oph_ip);
	
	while(1){
		if(bcp_is_alive(OPH)){
			break;
		}
	}
	printf("Connected to Ophiuchus, opening command log\n");
	snprintf(cmd_file,1024,"%s/cmd_logs_%ld.txt",gs_conf.cmd_log,time(NULL));
	cmd_log = fopen(cmd_file,"w");
	
	if(cmd_log == NULL){
                printf("Could not open command log %s:No such file or directory\n",cmd_file);
                exit(0);
	}
	
	printf("Command log open, accepting commands now\n");
	cmdprompt();
	fclose(cmd_log);

}
