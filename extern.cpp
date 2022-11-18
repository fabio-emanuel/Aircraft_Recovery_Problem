/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "variaveis.h"
extern bool alt_flights;
extern bool alt_arps;
extern bool alt_acfts;
extern bool pos_rest;
extern double min_gap;
extern vector < altflights_struct > altflts; 
extern vector < altslot_struct > altslots; 
extern vector < slot_struct > slotconsts; 
extern vector < altacft_struct > altacft; 
extern vector < string > namesga,namesfc,ynames; 
extern vector < int > lista_cfg;
extern char* input_arq,caminho;
extern recovery_time_struct recovery_time;
extern vector < rotation_struct > rots;
extern vector < aircraft_struct > acfts;
extern vector < config_struct > cfgs;
extern vector < int > cfgs_pos;
extern vector < flight_struct > flts;
extern vector < airport_struct > aerps;
extern vector < event_struct > evts;
extern vector < vector < vector < activity_struct > > > nos; 
extern vector < string > namesga; //names of the ground arcs by acft config and airpo
extern vector < int > conf_namesga,acfts_namesga, inic_gbga; //names of the ground arcs by acft config and airpo
extern vector < int > results_namesga,org_results_namesga; //names of the ground arcs by acft config and airpo
extern vector < altflights_struct > altflts; 
extern vector < altslot_struct > altslots; 
extern vector < altacft_struct > altacft; 
extern vector < vector < struct_gas > > gas;
extern string nome_output,nome_lp;
extern int result_1; // result of objective function of first problem;
extern string nome_output,nome_lp, nome_instancia, nome_lp_fifo;
extern int noptions;
extern int min_opt; // minutes by delay option
extern int spam_days; // maximum day number of the schedule
extern int minute_cost;
extern int cancel_cost;
extern int min_date;
extern int max_date;
extern int max_fln,max_fln_mnt;
extern double max_time;
extern vector < vector < int > > inf_inputs;
extern int pert_time;
extern int limit_hour_slot; 
extern vector < vector < int> > matrics;
extern vector < results_struct > arc_results;
extern vector < trilho_struct > trilhos;
extern int cont_mant;
extern int cont_mant_exec;
extern string arqresgeral;
extern vector < string > lps;
extern vector < flid_pos > flidpos;
extern vector < xfc_gb > vecgb_xfc;
extern vector < y_gb > vecgb_y;
extern vector < ga_gb > vecgb_ga;
extern vector < rodada_struct > res_rod;
extern vector < airpcap_struct > airpcap;


int flidtofln ( string flt_temp );
int arctofln ( string flt_temp );
int arctoflpos ( string flt_temp ) ;
int flidtoflpos(string flt_temp); 
int arctoopc(string buscado);
bool mant_arc (string arc);
bool mant_flid (string flid);
bool flid_alt_acft(string flid) ;
bool arc_result(string flt_temp);
int conf_flid(string flid);
int le_r1_fifo();
int result_ga(string ga);
void monta_fifo_heuristic();
int result_ga_gas(int cnf, string ga);
string esps(int cont);
string cplstr(string s, int l);
vector<string> split(string str, char delimiter);