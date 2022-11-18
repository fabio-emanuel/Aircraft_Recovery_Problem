#ifndef __JUNTA__H__
#define __JUNTA__H__


#include <ctime>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string> 
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <set>
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>
#include "variaveis.h"
#include "gurobi_c++.h"
 
vector<string> split(string str, char delimiter);
void read_instance(string caminho);
string int_hora(int tempo);    
void create_delay_opt( int noptions );
void create_flt_events();
void create_network();
string int_hora(int tempo);    
int le_r1();
int le_r2();

//int flidtofln ( string flt_temp );
int arctofln ( string flt_temp );
int arctoflpos ( string flt_temp ) ;
bool mant_arc (string arc);
bool mant_flid (string flid);
bool flid_alt_acft(string flid) ;
void create_lp() ;
void create_lp_fifo() ;
int le_r1_fifo();
int le_rout_ass();
void show_rotation(int result_1,string arqres);
void monta_fifo_heuristic();
void show_rotation_heuristic(string arqres);
void monta_R(string arqres);
void create_lp_fifo_separada();
rodada_struct create_lp_routing_assignment_gurobi2(string nome_lp, vector <int> lista_cfg_p);
rodada_struct create_lp_assignment_gurobi();
void create_lp_routing_gurobi2();
rodada_struct create_lp_routing_separada_gurobi2(rodada_struct temprod);
string cplstr(string s, int l);

//paramentros
bool pos_rest = true;
double max_time = 4*3600;
double min_gap = 0.03;
bool exact = false;
int min_opt = 15; // minutes by delay option
int minute_cost = 10;
int cancel_cost = 20000;


bool alt_flights = true;
bool alt_arps = true;
bool alt_acfts = true;
int noptions = 40;
int spam_days = 1; // maximum day number of the schedule
int min_date = 999;
int max_date = -1;
int max_fln,max_fln_mnt = 0;
int pert_time = 99999; //time of the first perturbation
int limit_hour_slot = -1;
recovery_time_struct recovery_time;
int result_1; // result of objective function of first problem;
char* input_arq,caminho;
string nome_output, nome_lp, nome_instancia, nome_lp_fifo;
int cont_mant = 0;
int cont_mant_exec = 0;

string arqresgeral = "./resultados.txt";

vector < int > lista_cfg;
vector < rotation_struct > rots;
vector < aircraft_struct > acfts;
vector < config_struct > cfgs;
vector < int > cfgs_pos;
vector < flight_struct > flts;
vector < airport_struct > aerps;
vector < event_struct > evts;
vector < vector < vector < activity_struct > > > nos; // nos [ configuration ] [ airport ] [ time ]
vector < string > namesga,namesfc,ynames; //names of the ground arcs by acft config and airpo
vector < int > conf_namesga,acfts_namesga,inic_gbga; //names of the ground arcs by acft config and airpo
vector < int > results_namesga,org_results_namesga; //names of the ground arcs by acft config and airpo
vector < altflights_struct > altflts; 
vector < altslot_struct > altslots; 
vector < slot_struct > slotconsts; 
vector < altacft_struct > altacft; 
vector < vector < int > > inf_inputs;
vector < vector < int> > matrics;
vector < trilho_struct > trilhos;
vector < vector < struct_gas > > gas;
vector < string > lps;

vector < flid_pos > flidpos;
vector < xfc_gb > vecgb_xfc;
vector < y_gb > vecgb_y;
vector < ga_gb > vecgb_ga;
vector < rodada_struct > res_rod;
vector < airpcap_struct > airpcap;



//vector < results_struct > arc_results;

void mostra_inputs(){
        int config;
        ofstream cout((char*)nome_output.c_str(), std::fstream::app);
        cout << "Aircraft file" << endl;
	    cout << "numero de linhas: " << acfts.size() << endl;
        cout << "acft_id  model  family  cfg  fh  op_cost  tat  tt  airp  mant  nflights" << endl;
        for (int i=0;i<acfts.size();i++) {
            config = acfts[i].cfg;
            cout << acfts[i].id << " " << cfgs[config].model << " " << cfgs[config].family << " " << cfgs[config].cfg << " " << acfts[i].fh << " " << acfts[i].op_cost << " " << acfts[i].tat << " " << acfts[i].tt << " " << acfts[i].airp << " " << acfts[i].mant << " " << acfts[i].nflights << endl;
        }
        cout << endl << "Configurations" << endl;
        cout << "i  id  model  family  cfg  ass  n_flights  cont_acfts" << endl;
        for (int i=0;i<cfgs.size();i++) {
            cout << i << " " << cfgs[i].id << " " << cfgs[i].model << " " << cfgs[i].family << " " << cfgs[i].cfg << " " << cfgs[i].ass << " " << cfgs[i].flts.size() << " ";
                        
            int cont_acft = 0;
            for (int j=0;j<aerps.size();j++) 
                cont_acft += aerps[j].in_acfts[i];
            cout << cont_acft;
            cout << endl;
        }
        
        
        cout << endl << "Flights file" << endl;
	cout << "numero de linhas: " << flts.size() << endl;
        cout << "flid  nflight  org  dest  dept  arrv  lastf  org_acft  maintenance" << endl;
        for (int i=0;i<flts.size();i++)
            cout << flts[i].flid << " " << flts[i].nflight << " " << flts[i].org << " " << flts[i].dest << " " << int_hora(flts[i].dept) << " " << int_hora(flts[i].arrv) << " " << flts[i].lastf << " " << flts[i].org_acft << " " << flts[i].mant  << endl;
        
        
	cout << endl << "Configuration file" << endl;
        cout << "d1  t1  d2  t2" << endl;
	cout << recovery_time.d1 << " " << int_hora(recovery_time.t1) << " " << recovery_time.d2 << " " << int_hora(recovery_time.t2) << endl;
	cout << "Recory End: " << int_hora(recovery_time.end) << endl;

	cout << endl << "Rotation file" << endl;
	cout << "numero de linhas: " << rots.size() << endl;
        cout << "d_flight  id_acft  cfg  flid  org  dept  dest  arrv" << endl;
        for (int i=0;i<rots.size();i++) {        
            string flt_temp = rots[i].flid;
            int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
            cout <<  rots[i].d_flight << " " << rots[i].id_acft << " " << rots[i].cfg << " " << rots[i].flid << " " << flts[flt_pos].org << " " << int_hora(flts[flt_pos].dept) << " " << flts[flt_pos].dest << " " << int_hora(flts[flt_pos].arrv) << endl;
        }
           
        cout << endl << "Airport file" << endl;
	cout << "numero de linhas: " << aerps.size() << endl;
        cout << "id nome [Hora: dep/h arrv/h]  in_acfts[cfg: #acfts" << endl;
	for (int i=0;i<aerps.size();i++){
            cout << i << " " << aerps[i].id << " " << aerps[i].nome << " ";
            for (int j=0;j<aerps[i].slot_lim.size();j++)
                cout << "H" << j << ": " << aerps[i].slot_lim[j][0] << " " << aerps[i].slot_lim[j][1] << " ";
            cout << "|";
            for (int j=0;j<cfgs.size();j++)
                cout << "C" << j << ": " << aerps[i].in_acfts[j] << " ";
            cout << endl;
        }
        
        cout << endl << "Position file" << endl;
        cout << "id  nome  acft_pos aerp_pos" << endl;
	for (int i=0;i<aerps.size();i++)
            if (aerps[i].acft_pos.size()>0) {
                cout << aerps[i].id << " " << aerps[i].nome << " " << aerps[i].acft_pos.size();
                int conf;
                for (int j=0;j<aerps[i].acft_pos.size();j++) {
                    conf = j;
                    //cout << " |" << conf << " " << aerps[i].acft_pos[j][1];
                    cout << " | " << cfgs[conf].model << " " << cfgs[conf].cfg << " " << aerps[i].acft_pos[j];
                }
                cout << endl; 
            }
        
        cout << endl << "Alt file: flighs" << endl;
	cout << "numero de linhas: " << altflts.size() << endl;
        cout << "d_flight  flid_pert  atr" << endl;
	for (int i=0;i<altflts.size();i++)
            if (altflts[i].atr>0)
                cout << i << " " << altflts[i].d_flight << " " << altflts[i].flid_pert << " " << altflts[i].atr << endl;
            else
                cout << i << " " << altflts[i].d_flight << " " << altflts[i].flid << " " << altflts[i].atr << endl;
            
        cout << endl << "Alt file: airports" << endl;
	cout << "numero de linhas: " << altslots.size() << endl;
        cout << "airp  arrv  dep  h1  h2" << endl;
	for (int i=0;i<altslots.size();i++)
            cout << i << " " << altslots[i].airp << " " << altslots[i].arrv << " " << altslots[i].dep << " " <<  altslots[i].h1 << " " << altslots[i].h2 << endl;
        
        cout << endl << "Alt file: aircrafts" << endl;
	cout << "numero de linhas: " << altacft.size() << endl;
        cout << "acft_id  d1  t1  d2  t2" << endl;
	for (int i=0;i<altacft.size();i++)
            cout << i << " " << altacft[i].acft_id << " " << altacft[i].d1 << " " << int_hora(altacft[i].t1) << " " <<  altacft[i].d2 << " " << int_hora(altacft[i].t2) << endl;
}

void show_flt_events() {
    ofstream cout((char*)nome_output.c_str(), std::fstream::app);
    //log create_flt_events
    cout << endl << " Flight Events " << endl;
    cout << "Numero de Eventos: " << evts.size() << endl;
    cout << "id   key  key_flight   local  hora  tipo" << endl;
    for (int i=0;i<evts.size();i++)
		if (evts[i].key_flight != "END_REVOVERY")
			cout << i << " " << evts[i].key << " " << evts[i].key_flight << " " << evts[i].local << " " << int_hora(evts[i].timee) << " " << evts[i].type << " " << endl;
}

string list_in_acft(int cfg, int aep) {
    string temps = "";
    for (int i = 0; i < aerps[aep].id_in_acfts[cfg].size(); i++)
        temps = temps + aerps[aep].id_in_acfts[cfg][i] + " ";
    return temps;
}

void show_network() {
    ofstream cout((char*)nome_output.c_str(), std::fstream::app);
    cout << endl << "Network " << endl << endl;
    for (int p=0;p<cfgs.size();p++) {
        cout << "Commodity: " << cfgs[p].model << " " << cfgs[p].cfg << endl << endl;
        for (int i=0;i<aerps.size();i++) {
            if (nos[p][i].size()>0) {
                cout << "Aeroporto: " << nos[p][i][0].local << endl << endl << " Number of Inpt Acfts: " << nos[p][i][0].in_acft << endl;
                
                for (int j=0;j<nos[p][i].size();j++){           
                    cout << nos[p][i][j].conf << "_" << nos[p][i][j].key << ": " << int_hora(nos[p][i][j].timee) << " ";     
                    cout << "Inflight: [";
                    for (int q=0;q<nos[p][i][j].infltarc.size();q++) { cout << nos[p][i][j].infltarc[q] << " ";  }
                    cout << "] Outflight: [";
                    vector < string > orgacft;
                    for (int q=0;q<nos[p][i][j].outfltarc.size();q++) { 
                        cout << nos[p][i][j].outfltarc[q] << " "; 
                        orgacft.push_back(nos[p][i][j].outfltarc[q]);
                    }
                    for (int q=0;q<orgacft.size();q++) {
                        string fltmp;
                        if (orgacft[q].size()>0)
                            fltmp=flts[arctoflpos(orgacft[q])].org_acft;
                        else
                            fltmp = "-";
                        
                    }
                    cout << "] InGrdArc: [";
                    for (int q=0;q<nos[p][i][j].ingrdarc.size();q++) {cout << nos[p][i][j].ingrdarc[q] << " "; }
                    cout << "] OutGrdArc: [";
                    for (int q=0;q<nos[p][i][j].outgrdarc.size();q++) { cout << nos[p][i][j].outgrdarc[q] << " " << nos[p][i][j].outgrdarc_int[q] << " ";  }
                    cout << "] " << endl;
                }
                cout << endl;
            }
        }
    }
}

void show_network_result() {
    ofstream cout((char*)nome_output.c_str(), std::fstream::app);
    cout << endl << "Network Results" << endl << endl;
    for (int p=0;p<cfgs.size();p++) {
        cout << "Commodity: " << cfgs[p].model << " " << cfgs[p].cfg << endl << endl;
        for (int i=0;i<aerps.size();i++) {
            if (nos[p][i].size()>0) {
                cout << "Aeroporto: " << nos[p][i][0].local << endl << endl << " Number of Inpt Acfts: " << nos[p][i][0].in_acft << endl;
                
                for (int j=0;j<nos[p][i].size();j++){           
                    cout << nos[p][i][j].conf << "_" << nos[p][i][j].key << ": " << int_hora(nos[p][i][j].timee) << " ";     
                    
					cout << "InGrdArc: [";
					for (int q = 0; q<nos[p][i][j].ingrdarc.size(); q++)
						if (results_namesga[nos[p][i][j].ingrdarc_int[q]]>0)
							cout << nos[p][i][j].ingrdarc[q] << "=" << results_namesga[nos[p][i][j].ingrdarc_int[q]];
					
					cout << "] Inflight: [";
					for (int q = 0; q < nos[p][i][j].infltarc.size(); q++) { 
						int fp = nos[p][i][j].infltarc_int[q].flidpos;
						if (nos[p][i][j].infltarc_int[q].opc == flts[fp].result)
							cout << nos[p][i][j].infltarc[q] << " ";
					}
                    
					
					cout << "] Outflight: [";
                    for (int q=0;q<nos[p][i][j].outfltarc.size();q++) { 
						int fp = nos[p][i][j].outfltarc_int[q].flidpos;
						if (nos[p][i][j].outfltarc_int[q].opc == flts[fp].result)
							cout << nos[p][i][j].outfltarc[q] << " ";
                    }
                    cout << "] OutGrdArc: [";
                    for (int q=0;q<nos[p][i][j].outgrdarc.size();q++)  
						if (results_namesga[nos[p][i][j].outgrdarc_int[q]]>0)
							cout << nos[p][i][j].outgrdarc[q] << "=" << results_namesga[nos[p][i][j].outgrdarc_int[q]];
					
                    cout << "] " << endl;
                }
                cout << endl;
            }
        }
    }
}

void roda_gurobi(){
    
    string comad("C:/gurobi751/win64/bin/gurobi_cl ResultFile=./Results/");
    comad = comad + nome_instancia + "/" + nome_instancia + ".sol " + nome_lp + ">> " + nome_output; 

    ofstream cout((char*)nome_output.c_str(), std::fstream::app);
    
    int i=system ((char*)comad.c_str());
    cout << endl << "The value returned was: " << i;
}

void roda_gurobi_fifo(){
    
    ofstream cout((char*)nome_output.c_str(), std::fstream::app);
    for (int i=0;i<lps.size();i++){
        string nomelp = lps[i];
        string nomeresult = lps[i].substr(0,lps[i].length()-3)+".sol";
        
        string comad("C:/gurobi751/win64/bin/gurobi_cl ResultFile=");
        comad = comad + nomeresult + " " + nomelp + ">> " + nome_output; 
        int k=system ((char*)comad.c_str());
        cout << endl << "The value returned was: " << i;
    }
}

void imprime_log(){
	ofstream res_file_gnr((char *)arqresgeral.c_str(), std::fstream::app);
	
	res_file_gnr << endl << cplstr("Exact: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++) {
		string tipos = "Exato";
		if (!exact) tipos = "MA";
		res_file_gnr << cplstr(tipos, 15);
	}
	res_file_gnr << endl;
	res_file_gnr << cplstr("Nome: ",20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(res_rod[i].Nome, 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NACFTs: ",20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NACFTS), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NFlights: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NFLIGHT), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("INIT_FLIGHT_DELAY: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].INIT_FLIGHT_DELAY), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NCANC: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NCANC), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("HOURS_CAP_RED: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].N_HOURS_AIRPORT_CAPACITY_REDUCTION), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("HOURS_ACFT_BROKEN: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].N_HOURS_ACFT_BROKEN_PERIOD), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("FEASIBLE: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].FEASIBLE), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("RESULT: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].RESULT_ASSIGN), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("GAP: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].GAP), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("TIME: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].Time), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("Status: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].Status), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NCANC_POS: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NCANC_POS), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NATZ15: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NATZ15), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NATZ30: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NATZ30), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NATZ60: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NATZ60), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NATZM60: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NATZM60), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("NSWAPS: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].NSWAPS), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("DIF_POS: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].dif_pos), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("MA_INT_ROUT: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].ma_double), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("Genrl_Feasib: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].gnr_feas), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("Resultado_GERAL: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].TOTAL_RESULT), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("Time_Atrib: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].timeatrib), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("Time_Rot: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].timerot), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("Time_Exact: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].timeexact), 15);
	res_file_gnr << endl;
	res_file_gnr << cplstr("Total_Time: ", 20);
	for (size_t i = 0; i < res_rod.size(); i++)
		res_file_gnr << cplstr(to_string(res_rod[i].Total_Time), 15);
	res_file_gnr << endl;

}

void limpa_variaveis() {
    lista_cfg.clear();
    rots.clear();
    acfts.clear();
    cfgs.clear();
    cfgs_pos.clear();
    flts.clear();
    aerps.clear();
    evts.clear();
    nos.clear(); // nos [ configuration ] [ airport ] [ time ]
    namesga.clear();
    namesfc.clear();
    ynames.clear(); //names of the ground arcs by acft config and airpo
    results_namesga.clear();
    conf_namesga.clear(); //names of the ground arcs by acft config and airpo
    acfts_namesga.clear();
    altflts.clear(); 
    altslots.clear(); 
    slotconsts.clear();    
	inic_gbga.clear();
    altacft.clear();
    pert_time = 99999;
    inf_inputs.clear();
    org_results_namesga.clear();
    matrics.clear();
    min_date = 9999;
    max_date = -9999;
    max_fln = 0;
    max_fln_mnt = 0;
    pert_time = 99999;
    limit_hour_slot = -1;
    trilhos.clear();
    gas.clear();
    lps.clear();
	flidpos.clear();
	vecgb_xfc.clear();
	vecgb_y.clear();
	vecgb_ga.clear();
	airpcap.clear();
    cont_mant = 0;
	cont_mant_exec = 0;
}

int main()
{

	time_t now_inicial = time(0);
    // configuracao da rodada
	
	ofstream res_file_gnr((char *)arqresgeral.c_str());
	res_file_gnr.close();
    ifstream arq_rodadas("./rodadas",ios::in);  
    
   //ofstream inputs_arq("inputs.txt",ios::out);
   //inputs_arq << "#acfts #cfgs #flights #aerps #\mants #pert_flight #pert_aerps #pert_acfts #dif_acfts" << endl; 
    
    string line("antes");
   
    string caminho0,caminho;
	int ex = 0;
	int mgap = 0;
	int maxtime = 0;
    int n;
	for (n = 0; getline(arq_rodadas, line); ++n) 
		if (line[1] == '#')	
			break;
	n -= 5;
    arq_rodadas.clear();
    arq_rodadas.seekg(0,ios::beg);
	getline(arq_rodadas, line);
	ex = stoi(line);
	if (ex == 0) exact = 0; else exact = 1;
	getline(arq_rodadas, line);
	mgap = stoi(line);
	getline(arq_rodadas, line);
	max_time = stod(line);
	getline(arq_rodadas, line);
	vector <string> sep = split(line, ';');
	min_opt = stoi(sep[0]);
	minute_cost = stoi(sep[1]);
	cancel_cost = stoi(sep[2]);

	min_gap = (double) mgap / (double) 100;

	cout << "Gap: " << min_gap << " Max_Time: " << max_time << " Minute Opt: " << min_opt << " Minute Cost: " << minute_cost << " Cancel Cost: " << cancel_cost << endl;
	string comad;
    for (int n_rod = 0; n_rod<n; n_rod++) {
        time_t now0 = time(0);
		rodada_struct temprod;
		temprod.exact = exact;
		getline(arq_rodadas, line);
        vector <string> sep = split(line, ';');
        caminho0 = sep[0];
        nome_instancia = sep[1];
        noptions = stoi(sep[2]);
        alt_flights = stoi(sep[3]);
        alt_arps = stoi(sep[4]);
        alt_acfts = stoi(sep[5]);
        caminho = caminho0 + nome_instancia ;
        comad = "./Results/";
        comad = comad + nome_instancia; 
		int mkdirResult = _mkdir(comad.c_str());
                
        nome_lp = "./Results/" + nome_instancia + "/" + nome_instancia + "_" + to_string(noptions) + ".lp";
        nome_lp_fifo = "./Results/" + nome_instancia + "/fifo_" + nome_instancia + "_" + to_string(noptions);
        nome_output = "./Results/"+ nome_instancia + "/output_" + nome_instancia + "_" + to_string(noptions) + ".txt";
        read_instance(caminho);
        time_t now1 = time(0);
		cout << "Rodada: " << n_rod+1 << "  " << nome_instancia << " Inicio" << endl;
        cout << caminho << endl << "Leitura Instancia Tempo: "  << now1-now0 << endl;
        mostra_inputs();
        cout << "Mostra Inputs" << endl;
        create_delay_opt(noptions);
        cout << "create_delay_opt" << endl;
        create_flt_events();
        cout << "create_flt_events" << endl;
        show_flt_events(); 
        //cout << "show_flt_events" << endl;
        time_t now2 = time(0);
        create_network();
        time_t now3 = time(0);
        cout << "create_network Tempo: " << now3 - now2 << endl;
        show_network();
        cout << "show_network" << endl;
        for (int i=0;i<cfgs.size();i++)
            if (cfgs[i].model!="TranspCom")
                lista_cfg.push_back(i);
		//lista_cfg.clear(); lista_cfg.push_back(3); lista_cfg.push_back(6); lista_cfg.push_back(18);
		//cout << "test3" << endl;
		time_t now53 = time(0);
		string arqres0 = "./Results/" + nome_instancia + "/origin_fifo_" + nome_instancia + "_" + to_string(noptions) + ".txt";
		show_rotation(0, arqres0);
		time_t now55 = time(0);
		cout << "Show Original Rotation Tempo: " << now55 - now53 << endl << endl;


        time_t now4 = time(0);


		if (!exact) {
			//Run MA Method

			//Fleet Assigment Problem
			temprod = create_lp_assignment_gurobi(); //MA
			// Creat LP Assignment Text
			//create_lp();
			time_t now5 = time(0); //MA
			temprod.timeatrib = now5 - now4;
			cout << endl << "create_lp and Run it Tempo: " << now5 - now4 << endl << endl; //MA
			show_network_result(); //MA
			// Run LP Assignment Text
			/*
			time_t now6_0 = time(0);
			roda_gurobi();
			time_t now6 = time(0);
			cout << "roda_gurobi Atribuicao Tempo: " << now6 - now6_0 << endl;
			int lres_inst = le_rout_ass();
			time_t now11 = time(0);
			cout << "Leitura Dados Fleet Assignment Tempo: " << now11 - now6 << endl;
			*/
			//End of Fleet Assigment Problem
		

			// Routing Problem	
			if (temprod.countsol > 0) {  //MA
				//create_lp_fifo_separada();
				//create_lp_routing_gurobi2();
				//int gnr_fea = 2;
				// int gnr_fea = create_lp_routing_separada_gurobi2(res_ass); //MA
				vector < int > confnamesorg = conf_namesga;
				vector < int > resultsnamesorg = results_namesga;
				rodada_struct temprodfifo = create_lp_routing_separada_gurobi2(temprod);
				time_t now_rot = time(0); //MA
				temprod = temprodfifo;
				temprod.timerot = now_rot - now5;
				conf_namesga = confnamesorg;
				results_namesga = resultsnamesorg;
					//		time_t now12 = time(0);
					  //     cout << "create_LP_FIFO Tempo: " << now12 - now11 << endl;
							//roda_gurobi_fifo(); 
							/*time_t now13 = time(0);
							cout << "roda Gurobi FIFO Tempo: " << now13 - now12 << endl;
							int lres_inst_fifo = le_r1_fifo();
							time_t now135 = time(0);
							cout << "Leitura Dados FIFO Tempo: " << now135 - now13 << endl;
							 //time_t now7 = time(0); */
							 //monta_fifo_heuristic();
							 //time_t now8 = time(0);
							 //cout << "create_FIFO_Heuristic Tempo: " << now8 - now7 << endl;
							 //cout << "Show_FIFO  Mant: " << cont_mant << "  Mant_Exec: " << cont_mant_exec << "  %: " << (float) cont_mant_exec/cont_mant << endl;*/
							 //Mostra Resultado Final
		

				if (temprod.gnr_feas != 3) {  //MA
					int lres_inst = 0; //MA
					string arqres = "./Results/" + nome_instancia + "/results_fifo_" + nome_instancia + "_" + to_string(noptions) + ".txt"; //MA
			//		show_rotation_heuristic(arqres);
					time_t now135 = time(0); //MA
					show_rotation(0, arqres); //MA
					time_t now136 = time(0); //MA
					cout << "Show Final Rotation Tempo: " << now136 - now135 << endl; //MA
				}  //MA
				if (temprod.gnr_feas == 0) {
					//find configuration that didnt resolve optimaly and those that did
					vector < int > lista_cfg_0, lista_cfg_1;
					for (size_t i = 0; i < temprod.feasvec.size(); i++)
					{
						string cfg0 = temprod.confvec[i];
						int cfgpos = distance(cfgs.begin(), find_if(cfgs.begin(), cfgs.end(), [cfg0](const config_struct& a) { return a.model + to_string(a.ass) == cfg0; }));
						if (temprod.feasvec[i] == 0)
							lista_cfg_0.push_back(cfgpos);
						else
							lista_cfg_1.push_back(cfgpos);
					}

					// diminish airport capacity using flights that were resolved optimaly
					for (size_t i = 0; i < flts.size(); i++)
						if ((!flts[i].mant)&&(find(lista_cfg_1.begin(), lista_cfg_1.end(),flts[i].cfg)!= lista_cfg_1.end())&&(flts[i].result>=0)) {
							//diminish departure capacity
							int dephour = (flts[i].dept + min_opt*flts[i].result)/60;
							string deploc = flts[i].org;
							int deppos = distance(aerps.begin(), find_if(aerps.begin(), aerps.end(), [deploc](const airport_struct& a) { return a.nome == deploc; }));
							airpcap[deppos].depcap[dephour]--;
							//diminish arrival capacity 
							int arrvhour = (flts[i].arrv + min_opt*flts[i].result) / 60;
							string arrvloc = flts[i].dest;
							int arrvpos = distance(aerps.begin(), find_if(aerps.begin(), aerps.end(), [arrvloc](const airport_struct& a) { return a.nome == arrvloc; }));
							airpcap[arrvpos].arrvcap[arrvhour]--;
						}

					slotconsts.clear();
					inic_gbga.clear();
					vector <int> lista_cfg_org = lista_cfg;
					//lista_cfg = lista_cfg_0;

					string nome_lp_inf = "./Results/" + nome_instancia + "/" + nome_instancia + "_" + to_string(noptions) + "_inf.lp";
					rodada_struct temprod3 = create_lp_routing_assignment_gurobi2(nome_lp_inf, lista_cfg_0);
					
					// Resultado Caso o Modelo Integrado Rodar
					temprod.gnr_feas = temprod3.gnr_feas;
					if (temprod3.gnr_feas == 1) {
						//Contabiliza resultado Geral
						int res_tot = 0;
						int nswap = 0;
						int n15 = 0, n30 = 0, n60 = 0, nm60 = 0, final_delay = 0, ncanc = 0;
						for (int p = 0; p < flts.size(); p++)
							if (find(lista_cfg.begin(), lista_cfg.end(), flts[p].cfg) != lista_cfg.end())
								if (flts[p].result < 0) {
									res_tot += cancel_cost;
									flts[p].cost = cancel_cost;
									ncanc++;
								}
								else {
									res_tot += minute_cost * min_opt * flts[p].result;
									flts[p].cost += minute_cost * min_opt * flts[p].result;
									if (flts[p].final_acft != flts[p].org_acft) {
										res_tot += 1;
										nswap++;
										flts[p].cost += 1;
									}
									if (flts[p].result == 1) n15++;
									if (flts[p].result == 2) n30++;
									if ((flts[p].result == 4) || (flts[p].result == 3)) n60++;
									if (flts[p].result > 4) nm60++;
									final_delay += min_opt * flts[p].result;
								}

						cout << endl << "Calculate non FINAL positioned airport" << endl;
						// Calculate non positioned airport
						int dif_pos = 0;
						for (int p = 0; p < lista_cfg.size(); p++) {
							int it_cfg = lista_cfg[p];
							for (int i = 0; i < nos[it_cfg].size(); i++) {
								if (nos[it_cfg][i].size() > 0) {
									int orgpos = 0;
									if (aerps[i].acft_pos.size() > 0)
										orgpos = aerps[i].acft_pos[it_cfg];
									int timeend = recovery_time.end + cfgs[it_cfg].tat;
									int posrcv = distance(nos[it_cfg][i].begin(), find_if(nos[it_cfg][i].begin(), nos[it_cfg][i].end(), [timeend](const activity_struct& a) { return a.timee == timeend; }));
									int realpos = nos[it_cfg][i][posrcv].outgrdarc_int[0];
									dif_pos += abs(results_namesga[realpos] - orgpos);
									cout << it_cfg << " " << nos[it_cfg][i][0].local << " " << orgpos << " " << results_namesga[realpos] << endl;
								}
							}
						}
						cout << endl;

						temprod.TOTAL_RESULT = res_tot;
						temprod.NSWAPS = nswap;
						temprod.NATZ15 = n15;
						temprod.NATZ30 = n30;
						temprod.NATZ60 = n60;
						temprod.NATZM60 = nm60;
						temprod.NCANC_POS = ncanc;
						temprod.gnr_feas = 1;
						temprod.dif_pos = dif_pos;
						if (temprod.GAP < temprod3.GAP)
							temprod.GAP = temprod3.GAP;
						temprod.ma_double = 1;
						int lres_inst = 0; //MA
						string arqres = "./Results/" + nome_instancia + "/results_fifo2_" + nome_instancia + "_" + to_string(noptions) + ".txt"; //MA
																																				//		show_rotation_heuristic(arqres);
						time_t now135 = time(0); //MA
						show_rotation(0, arqres); //MA
						time_t now136 = time(0); //MA
						cout << "Show Final Rotation 2 Tempo: " << now136 - now135 << endl; //MA
					}
					time_t now_exact = time(0); //MA
					temprod.timeexact = now_exact - now_rot;
				}
				

				
			}   //MA
			// End of Routing Problem
		}
		else {
			//  Run Exact Model
			vector <int> lista_cfg2;
			for (int i = 0; i<cfgs.size(); i++)
				if (cfgs[i].model != "TranspCom")
					lista_cfg2.push_back(i);
			temprod = create_lp_routing_assignment_gurobi2(nome_lp, lista_cfg2);
			//Mostra Resultado Final
			string arqres = "./Results/" + nome_instancia + "/results_fifo_" + nome_instancia + "_" + to_string(noptions) + ".txt";
			time_t now135 = time(0);
			show_rotation(0, arqres);
			time_t now136 = time(0);
			temprod.timeexact = now136 - now4;
			cout << "Show Final Rotation Tempo: " << now136 - now135 << endl;
		}

		res_rod.push_back(temprod);
		int buscado = 22;
		int nacf = count_if(acfts.begin(), acfts.end(),[buscado] (const aircraft_struct& a) {  return find(lista_cfg.begin(), lista_cfg.end(), a.cfg) != lista_cfg.end();});
		res_rod[res_rod.size() - 1].NACFTS = nacf;
		
		// Arquivo R
		string arqres2 = "./Results/" + nome_instancia + "/trilhosR.txt";
		monta_R(arqres2);



		limpa_variaveis();
		cout << "Limpa Variaveis" << endl;
		time_t now9 = time(0);
		

			
			// cria copia de arquivo
			ifstream source("./gantt.R", ios::binary);
			string arqres4 = "./Results/" + nome_instancia + "/gantt.R";
			ofstream dest(arqres4.c_str(), ios::binary);
			dest << source.rdbuf();
			source.close();
			dest.close();
			//string comady = "C:/Users/fabiomorais/Documents/R/R-3.4.1/bin/rscript ./Results/" + nome_instancia + "/gantt.R";
			//int roda = system ((char*)comady.c_str());*/
		
			ofstream res_file_gnr((char *)arqresgeral.c_str(), std::fstream::app);
		res_file_gnr << "Total_Time: " << now9 - now0 << endl << endl;
		res_file_gnr.close();
		cout << "Rodada: " << n_rod + 1 << "  " << nome_instancia << " Fim Tempo:" << now9 - now0 << endl << endl;
		res_rod[res_rod.size() - 1].Total_Time = now9 - now0;
		res_rod[res_rod.size() - 1].Nome = nome_instancia;
    }
	time_t now_final = time(0);	
    arq_rodadas.close();
	ofstream res_file_gnr2((char *)arqresgeral.c_str(), std::fstream::app);
	res_file_gnr2 << endl << "Total_Time: " << now_final - now_inicial;
	res_file_gnr2.close();
	imprime_log();
    return 0;
	
}

#endif

