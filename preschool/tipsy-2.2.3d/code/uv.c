#include "defs.h"
#include "fdefs.h"

void
uv(job)
    char *job;
{
    char command[MAXCOMM] ;
    char uv_type[MAXCOMM] ; 
    char filename[MAXCOMM] ;
    FILE *infile;
    double c1,c2,c3,c4,c5,c6 ;
    int number ;
    struct uv_source *uvs, *lastuvs ;

    if((sscanf(job,"%s %s %s", command, uv_type,filename) == 3) &&
	    ((strcmp(uv_type, "source") == 0 || strcmp(uv_type, "s") == 0))){
	infile = fopen(filename, "r");
	if(infile == NULL){
	    printf("<Sorry %s, source file does not exist>\n",title);
	    return ;
	}
	if (fscanf(infile,"%d",&source_num) != 1){
	    printf("<Sorry %s, source file format is wrong>\n",title);
	    return ;
	}
	if(uv_sources != NULL) free(uv_sources);
	uv_sources = 
		(struct uv_source *) malloc((source_num+1)*sizeof(*uv_sources));
	if(uv_sources == NULL) {
	    printf("<sorry, no memory for uv sources, %s>\n",title) ;
	    return ;
	}
	uvs = uv_sources ;
	if (fscanf(infile,"%lf %lf %lf %lf %lf %lf",&uvs->gp0_H,&uvs->gp0_He,
		&uvs->gp0_Hep,&uvs->eps_H,&uvs->eps_He,&uvs->eps_Hep) != 6){
	    printf("<Sorry %s, source file format is wrong>\n",title);
	}
	lastuvs = uv_sources + source_num + 1 ;
	for(uvs = uv_sources + 1 ; uvs < lastuvs; uvs++){
	    if (fscanf(infile,"%f %f %f %lf %lf %lf %lf %lf %lf",&uvs->pos[0],
		    &uvs->pos[1],&uvs->pos[2],&uvs->gp0_H,&uvs->gp0_He,
		    &uvs->gp0_Hep,&uvs->eps_H,&uvs->eps_He,&uvs->eps_Hep) != 9){
		printf("<Sorry %s, source file format is wrong>\n",title);
		return ;
	    }
	}
	uv_loaded = YES ;
	hneutral_loaded = NO ;
	cooling_loaded = NO ;
	meanmwt_loaded = NO ;
	uniform = NO ;
    }
    else if(((number = sscanf(job,"%s %s %lf %lf %lf %lf %lf %lf", command, 
	      uv_type,&c1,&c2,&c3,&c4,&c5,&c6)) == 4 || number == 8) &&
	      ((strcmp(uv_type, "uniform") == 0 || strcmp(uv_type, "u") == 0))){
	if(number == 4){
	    jnu21 = c1 ;
	    alphaj = c2 ;
	    if(jnu21 > 0.0){
		ionize() ;
		gp0_H *= jnu21 * 4 * PI * 1.e-21 ;
		gp0_He *= jnu21 * 4 * PI * 1.e-21 ;
		gp0_Hep *= jnu21 * 4 * PI * 1.e-21 ;
		eps_H *= jnu21 * 4 * PI * 1.e-21 ;
		eps_He *= jnu21 * 4 * PI * 1.e-21 ;
		eps_Hep *= jnu21 * 4 * PI * 1.e-21 ;
	    }
	    else{
		gp0_H = 0.0 ;
		gp0_He = 0.0 ;
		gp0_Hep = 0.0 ;
		eps_H = 0.0 ;
		eps_He = 0.0 ;
		eps_Hep = 0.0 ;
	    }
	}
	else{
            gp0_H = c1 ;
	    gp0_He = c2 ;
	    gp0_Hep = c3 ;
	    eps_H = c4 ;
	    eps_He = c5 ;
	    eps_Hep = c6 ;
	}
	uv_loaded = YES ;
	hneutral_loaded = NO ;
	uniform = YES ;
	cooling_loaded = NO ;
	meanmwt_loaded = NO ;
	starform_loaded = NO ;
	dudt_loaded = NO ;
    }
    else {
	input_error(command) ;
    }
}
