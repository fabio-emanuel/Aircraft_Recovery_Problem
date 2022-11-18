#include "variaveis.h"
#include "extern.cpp"

int result_ga_gas(int cnf, string ga);

int pos_quebra2(string str,int pos) {
    int str1 = str.find_first_of(" ",pos-5);
    int a;
    if (str1!=str.npos) {
        a = str1;
        return a;
    }
    else
        return 0;
}


string put_lines2(string junta){
    int ant = 0;
    int cont = 0;
    string tempstr("");
    int step = 200;
    int quebras = junta.length()/step;
    int pos=step;
    int tam = junta.length();
    while (tam>step) {
    //for ( int i=0;i<quebras;i++ ) {    
                pos = pos_quebra2(junta,pos);
                int cont = pos - ant;
                string temp2 = junta.substr(ant,cont);
                tempstr += temp2;
                tempstr += "\n";
                ant = pos;
                pos += step;
                tam -= temp2.length();
    }
    tempstr += junta.substr(ant,junta.length()-ant);     
    return tempstr;            
}

// FLEET ASSIGNMENT TEXT FILE
void create_lp() {


        ofstream lp((char*)nome_lp.c_str());
       
        
        ofstream cout((char*)nome_output.c_str(), std::fstream::app);
        
        cout << "LP construction" << endl << endl;
        
        lp << "\\ Model " << nome_instancia << endl << "\\Numero de Opcoes de Delay: " << noptions << endl;
        
        string junta_text;
        string string_bin="";
        int nflights = 0;
        for (int i=0; i<flts.size(); i++) 
            if (find(lista_cfg.begin(),lista_cfg.end(),flts[i].cfg)!=lista_cfg.end()) {
                if (!flts[i].mant) {
                    string varName = "yf_" + to_string(flts[i].cfg) + "_" + flts[i].flid;
                    //yf[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
                    ynames.push_back(varName);
                    //string_bin = text_output_limit(string_bin,varName,240);
                    nflights++;
                }
            }
	
        lp << "\\ Number of Configurationd: " << lista_cfg.size() << " - All but Surface Flight -  Number of Flights: " << nflights << endl << endl;
        lp << "Minimize" << endl;
        lp << "  ";
        
        
        cout << "yf variables added" << endl;
        
        int h = 0;
        //int it_cfg; 
        
        for (int p=0;p<lista_cfg.size();p++){
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){
                string flt_temp = cfgs[it_cfg].fltids[i];
                int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                for (int j=0;j<flts[flt_pos].kfs.size();j++){
                    string varName = "xfc_" + to_string(flts[flt_pos].cfg) + "_" + flts[flt_pos].kfs[j];
                    //xfc[h] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
                    namesfc.push_back(varName);
                    h++;
                    string_bin += varName +" ";
                    //string_bin += varName + "\n";
                    //string_bin = text_output_limit(string_bin,varName,240);
                }
            }    
        }
        cout << "xfc variables added" << endl;     
        
        string string_int="";
        for (int p=0;p<namesga.size();p++) {
            if (find(lista_cfg.begin(),lista_cfg.end(),conf_namesga[p])!=lista_cfg.end()) {
                string varName = namesga[p]; 
                //zga[p] = model.addVar(0, 1000000, 0, GRB_INTEGER, varName);
        
                string_int += varName+" ";
                //string_int += varName+"\n";
                //string_int = text_output_limit(string_int,varName,240);
            }
        }
        cout << "zga variables added" << endl;
        
//        model.update();
        
	
        //Balance Flow Constraint  
        
        vector < string >  string_rest;
        
        for (int p=0;p<lista_cfg.size();p++) {
            int it_cfg = lista_cfg[p];
            for (int i=0;i<nos[it_cfg].size();i++)
                for (int k=0;k<nos[it_cfg][i].size();k++) {
                    if (nos[it_cfg][i][k].key.substr(0,3)!="OUT") {
                        vector <string> textao;
                        string texto ="";
                        string constraintName = "BF_" + to_string(it_cfg) + "_" + nos[it_cfg][i][k].key;
                        texto += " " + constraintName + ": ";
                        //GRBLinExpr expr = 0;
    //                    expr += nos[p][i][k].in_acft;

        
                        for (int j=0;j<nos[it_cfg][i][k].infltarc.size();j++) {
                            string flt_temp = nos[it_cfg][i][k].infltarc[j];
                            string busc = "xfc_" + to_string(it_cfg) + "_" + flt_temp;
                            //int flt_pos = distance(namesfc.begin(),find_if(namesfc.begin(),namesfc.end(), [busc] (const string& a) { return a==busc; }));
                            //expr += xfc[flt_pos];
                            texto +=  " + " + busc;
        
                        }
        
                        for (int j=0;j<nos[it_cfg][i][k].ingrdarc.size();j++) {
                            string flt_temp = nos[it_cfg][i][k].ingrdarc[j];
                            int flt_pos = distance(namesga.begin(),find_if(namesga.begin(),namesga.end(), [flt_temp] (const string& a) { return a==flt_temp; }));
                            //expr += zga[flt_pos];
                            texto +=  " + " + namesga[flt_pos];
        
                        }
        
                        for (int j=0;j<nos[it_cfg][i][k].outfltarc.size();j++) {
                            string flt_temp = nos[it_cfg][i][k].outfltarc[j];
                            string busc = "xfc_" + to_string(it_cfg) + "_" +flt_temp;
                            //int flt_pos = distance(namesfc.begin(),find_if(namesfc.begin(),namesfc.end(), [busc] (const string& a) { return a==busc; }));
                            //expr -= xfc[flt_pos];
                            texto +=  " - " + busc;
                        }
        
                        for (int j=0;j<nos[it_cfg][i][k].outgrdarc.size();j++) {
                            string flt_temp = nos[it_cfg][i][k].outgrdarc[j];
                            int flt_pos = distance(namesga.begin(),find_if(namesga.begin(),namesga.end(), [flt_temp] (const string& a) { return a==flt_temp; }));
                            //expr -= zga[flt_pos];
                            texto +=  " - " + namesga[flt_pos];
                        }
        
                        //model.addConstr(expr == 0, constraintName); 
                        texto += " = " + to_string(-nos[it_cfg][i][k].in_acft);
                        string_rest.push_back(texto);
                    }
                }
	}
        
        cout << "Balance Flow Constraint Added" << endl;
        
        //Cancel or Delay Constraints
        for (int p=0;p<lista_cfg.size();p++) {
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){   
                string texto ="";
                
                string flt_temp = cfgs[it_cfg].fltids[i];
                string constraintName = "CancelDelay_" + to_string(it_cfg) + "_" + flt_temp;
                texto += " " + constraintName + ": ";
        
                //GRBLinExpr expr = 0;
                if (!mant_flid(flt_temp)) {    
                    string varName = "yf_" + to_string(it_cfg) + "_" + flt_temp;
                    //int name_pos = distance(ynames.begin(),find_if(ynames.begin(),ynames.end(), [varName] ( const string& a){ return a == varName; }));
        
                    //expr += yf[name_pos];
                    int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                    
                    //flight whose dep is earlier than beginning of recovery time is unchanged
                    
                    if ((flts[flt_pos].dept < pert_time)&&(pert_time!=99999)) {
                        constraintName = "UnaffectedFlight_" + to_string(it_cfg) + "_" + flt_temp;
                        texto = " " + constraintName + ": ";
                    }
                    else
                        texto += varName;
                        
                    
                    
                    for (int j=0;j<flts[flt_pos].kfs.size();j++) {
                        string fl_temp = flts[flt_pos].kfs[j];
                        string busc = "xfc_" + to_string(it_cfg) + "_" + fl_temp;
        
                        //int name_pos = distance(namesfc.begin(),find_if(namesfc.begin(),namesfc.end(), [busc] (const string& a) { return a==busc;}));
                        //expr += xfc[name_pos];
                        texto +=  " + " + busc;
                    }
                    texto += " = 1";
                    string_rest.push_back(texto);
                }
                //model.addConstr(expr == 1, constraintName);      
            }
        }
        
        cout << "Coverture - Cancel or Delay Constraints" << endl;
        
        
        //Slot Capacity Constraint
        
        
        for (int i=0;i<evts.size();i++) {
            int evt_cnf = evts[i].cnfg;
            if (!mant_arc(evts[i].key_flight)&&(find(lista_cfg.begin(),lista_cfg.end(),evt_cnf)!=lista_cfg.end())) {    
                slot_struct temp_slot;
                string tp; 
                int tipo_cap_slot;
                if (evts[i].type) {
                    temp_slot.tipo = "d"; 
                    tipo_cap_slot = 0;
                }
                else {
                    temp_slot.tipo = "a";
                    tipo_cap_slot = 1;
                }
                temp_slot.key_slot = evts[i].local + to_string(evts[i].h_slot) + temp_slot.tipo;
                int h_busca = evts[i].h_slot % 24;
                string aep_temp = evts[i].local;
                int aep = distance(aerps.begin(),find_if(aerps.begin(),aerps.end(), [aep_temp] (const airport_struct& a) { return a.nome==aep_temp; }));
                temp_slot.rest = aerps[aep].slot_lim[h_busca][tipo_cap_slot];
                string busc = " + xfc_" +  to_string(evts[i].cnfg) + "_" + evts[i].key_flight;
                temp_slot.constraint = "Slot_" + temp_slot.key_slot + " : " + busc;
                string key_temp = temp_slot.key_slot;
                auto it = find_if(slotconsts.begin(),slotconsts.end(), [key_temp] (const slot_struct& a) { return a.key_slot==key_temp; });
                if (it==slotconsts.end())                
                    slotconsts.push_back(temp_slot);
                else {
                    int pos = distance(slotconsts.begin(),it);
                    slotconsts[pos].constraint += busc; 
                }
            }
        }
        //for (int i=0;i<slotconsts.size();i++) 
        //    slotconsts[i].constraint = "Slot_" + slotconsts[i].key_slot + " : " + slotconsts[i].constraint + " <= " + to_string(slotconsts[i].rest); 
        
        cout << "Slot Capacity Constraint" << endl; 
        
        //Maintenance Constraint
        for (int p=0;p<lista_cfg.size();p++){
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){   
                string texto ="";
                string flt_temp = cfgs[it_cfg].fltids[i];
                
                string constraintName;
                if ((mant_flid(flt_temp))&&(!flid_alt_acft(flt_temp))) {
                    constraintName = "Maintenance_" +  to_string(it_cfg)  + "_" + flt_temp;
                    texto += " " + constraintName + ": ";
                    
                    int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                    string fl_temp = flts[flt_pos].kfs[0];
                    string busc = "xfc_"  + to_string(it_cfg) + "_" + fl_temp;
                      //  int name_pos = distance(namesfc.begin(),find_if(namesfc.begin(),namesfc.end(), [busc] (const string& a) { return a==busc; }));
                     //   expr += xfc[name_pos];    
                    texto += busc + " = 1";
                        // model.addConstr(expr == 1, constraintName); 
                    string_rest.push_back(texto);
                }   
            }	
	}
        //model.update();
	cout << "Maintenace Constraint" << endl;		
	// Objective Function
	 
        if (alt_acfts) 
            for (int i=0;i<altacft.size();i++) { 
                int it_cfg = altacft[i].cfg;
                if (find(lista_cfg.begin(),lista_cfg.end(),it_cfg)!=lista_cfg.end()) {                    
                    string texto ="";
                    string constraintName = "Acft_unavailabe_" +  to_string(it_cfg)  + "_" + to_string(i);                    
                    texto += constraintName + ": ";
                    for (int j=0; j<altacft[i].flids.size();j++)
                        texto += " + xfc_"  + to_string(it_cfg) + "_" + altacft[i].flids[j] + "_0";
                    texto += " = 1";
                    string_rest.push_back(texto);
                }   
            }	           
        
        cout << "Aircraft Unavailable Constraint" << endl;		
             
        if (alt_flights)
            for (int i = 0; i<altflts.size(); i++){
                int it_cfg = conf_flid(altflts[i].flid);
                if (find(lista_cfg.begin(),lista_cfg.end(),it_cfg)!=lista_cfg.end()) {                  
                    string flt_temp = altflts[i].flid;
                    string texto = "";
                    int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                    string constraintName = "Altflight_" +  to_string(flts[flt_pos].cfg)  + "_" + altflts[i].flid;
                    texto += " " + constraintName + ": ";
                    if (altflts[i].atr>0) {
                        //texto += " + yf_" + to_string(flts[flt_pos].cfg) + "_" + altflts[i].flid;
                        for (int j=0;j<altflts[i].atr_int;j++) 
                            texto += " + xfc_"  + to_string(flts[flt_pos].cfg) + "_" + altflts[i].flid + "_" + to_string(j);
                        texto += " = 0";
                    }
                    else {
                        texto += "yf_" + to_string(flts[flt_pos].cfg) + "_" + altflts[i].flid;
                        texto += " = 1";
                    }
                    string_rest.push_back(texto);
                }
            }
		cout << "Flight Perturbations Constraint" << endl;
        if (alt_arps) 
            for (int i=0;i<altslots.size();i++){
                for (int horaslot = altslots[i].h1; horaslot < altslots[i].h2; horaslot++) {                   
                    // departue
                    string keyslot = altslots[i].airp + to_string(horaslot) + "d";
                    auto its = find_if(slotconsts.begin(),slotconsts.end(), [keyslot] (const slot_struct& a) { return a.key_slot==keyslot; }); 
                    if ( its!=slotconsts.end() ) {
                        int slot_pos = distance(slotconsts.begin(),its);
                        slotconsts[slot_pos].rest = altslots[i].dep;
                        slotconsts[slot_pos].constraint = "Alt_" + slotconsts[slot_pos].constraint;
                    }                   
                    // arrival
                    keyslot = altslots[i].airp + to_string(horaslot) + "a";
                    its = find_if(slotconsts.begin(),slotconsts.end(), [keyslot] (const slot_struct& a) { return a.key_slot==keyslot; }); 
                    if ( its!=slotconsts.end() ) {
                        int slot_pos = distance(slotconsts.begin(),its);
                        slotconsts[slot_pos].rest = altslots[i].arrv;
                        slotconsts[slot_pos].constraint = "Alt_" + slotconsts[slot_pos].constraint;
                    }
                    
                }
            } 
		cout << "Airport Perturbations Constraint" << endl;
        //for (int i=0;i<slotconsts.size();i++) 
         //   slotconsts[i].constraint = "Slot_" + slotconsts[i].key_slot + " : " + slotconsts[i].constraint + " <= " + to_string(slotconsts[i].rest); 

        
        for (int i=0;i<slotconsts.size();i++)
            string_rest.push_back(slotconsts[i].constraint + " <= " + to_string(slotconsts[i].rest));
            
            
           
        //int cancel_cost = 2000;
        //int delay_cost = 5;
        //GRBLinExpr expr_obj = 0;
        string str_obj("");
        lp << " ";
        for (int p=0;p<lista_cfg.size();p++) {
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){   
                string flt_temp = cfgs[it_cfg].fltids[i];
                              
                if (!mant_flid(flt_temp)) { 
                
                    string varName = "yf_" + to_string(it_cfg) + "_" + flt_temp;
                    int name_pos = distance(ynames.begin(),find_if(ynames.begin(),ynames.end(), [varName] ( const string& a){ return a == varName; }));
          //          expr_obj += yf[name_pos]*cancel_cost;
                    lp << " + " + to_string(cancel_cost) + " " + varName;
                    
                    //lp << " + " + to_string(cancel_cost) + " " + varName << endl;
                   // str_obj = text_output_limit(str_obj,"+ " + to_string(cancel_cost) + " " + varName,240);
                    int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                    for (int j=0;j<flts[flt_pos].kfs.size();j++) {
                        string fl_temp = flts[flt_pos].kfs[j];
                        string busc = "xfc_" + to_string(it_cfg) + "_" + fl_temp;
                        int delay = stoi(fl_temp.substr(fl_temp.find("_")+1,fl_temp.npos));
                        //int name_pos = distance(namesfc.begin(),find_if(namesfc.begin(),namesfc.end(), [busc] (const string& a) { return a==busc; }));
            //          expr_obj += xfc[name_pos]*(delay_cost*30);
                        if (delay>0) {
                            lp << " + " + to_string(minute_cost*min_opt*delay) + " " + busc;
                            //lp << " + " + to_string(minute_cost*min_opt*delay) + " " + busc << endl;
                           // str_obj = text_output_limit(str_obj,"+ " + to_string(minute_cost*min_opt*delay) + " " + busc,240);
                        }
                    }
                }
            }
        }
        
        lp << str_obj << endl;
        lp << endl << "Subject To" << endl;
        for (int i=0;i<string_rest.size();i++) lp << string_rest[i] << endl;
        lp << "Bounds" << endl << "Binaries" << endl << string_bin << endl << "Generals" << endl << string_int << endl << "End" << endl;
        
	cout << "Objective function added" << endl;
	//model.setObjective(expr_obj,GRB_MINIMIZE);
//	model.update();
//	model.write("Recovery_" + nome_instancia + ".lp");	  
        time_t now1 = time(0);
        lp.close();
    
}

string name_voo(int cfg, string flid, int result, int acft ) {
    string temp_arc = flid + "_" + to_string(result);
    string varName = "xfc_" + to_string(cfg) + "_" + temp_arc + "_" + to_string(acft);
    return varName;
}

string name_input(int cfg, int aep, int acft) {
    return "nin_" + to_string(cfg) + "_" + to_string(aep) +  "_" + to_string(acft);
} 

string BF_str (int it_cfg, int i, int k, int mat  ) {
  vector <string> textao;
  string texto ="";
  string constraintName = "BF_" + to_string(it_cfg) + "_" + nos[it_cfg][i][k].key + "_" + to_string(mat);
  texto += " " + constraintName + ": ";
  string texto2("");
  

  
  for (int j=0;j<nos[it_cfg][i][k].infltarc.size();j++) {
      string flt_temp = nos[it_cfg][i][k].infltarc[j];         
      if (arc_result(flt_temp)){
          string busc = "xfc_" + to_string(it_cfg) + "_" + flt_temp + "_" + to_string(mat);        
          texto2 +=  " + " + busc;
      }
  }

  for (int j=0;j<nos[it_cfg][i][k].ingrdarc.size();j++) {
        if (result_ga(nos[it_cfg][i][k].ingrdarc[j]) > 0) {
            string flt_temp = nos[it_cfg][i][k].ingrdarc[j] + "_" + to_string(mat);
            texto2 +=  " + " + flt_temp;
        }
  }

  for (int j=0;j<nos[it_cfg][i][k].outfltarc.size();j++) {
      string flt_temp = nos[it_cfg][i][k].outfltarc[j];
      if (arc_result(flt_temp)){
          string busc = "xfc_" + to_string(it_cfg) + "_" + flt_temp + "_" + to_string(mat); 
          texto2 +=  " - " + busc;
      }
  }

  for (int j=0;j<nos[it_cfg][i][k].outgrdarc.size();j++) {
      if (result_ga(nos[it_cfg][i][k].outgrdarc[j]) > 0) {
        string flt_temp = nos[it_cfg][i][k].outgrdarc[j] + "_" + to_string(mat);
        texto2 +=  " - " + flt_temp;
      }
  }


  if (texto2.size()) {
    if ((acfts[mat].airp==nos[it_cfg][i][k].local)&&(k==0))    
        texto += texto2 + " = - 1";
    else
        texto += texto2 + " = 0";
  }
  else
    texto = ""; 
  
  
          
  //model.addConstr(expr == 0, constraintName); 
  
  return texto;


    
}

// ROUTING ALL CONFIGURATIONS TOGETHER TEXT FILE
void create_lp_fifo() {


        ofstream lp((char*)nome_lp_fifo.c_str());
        ofstream cout((char*)nome_output.c_str(), std::fstream::app);
        
        cout << "LP construction" << endl << endl;
        
        lp << "\\ Routing Linear Programming " << nome_instancia << endl;
        
        // cria vetor com as matriculas
        //lp << "test1" << endl;
        
        for (int i = 0; i < lista_cfg.size(); i++) {
            int id_cfg = lista_cfg[i];
            vector < int > temp_matrics;
            for (int j = 0; j < acfts.size(); j++)
                if (acfts[j].cfg == id_cfg)
                    temp_matrics.push_back(j);
            matrics.push_back(temp_matrics);
        }  
        
        
        
        
        /*lp << "test2" << endl;
        // cria vetor com os nos de entrada
        vector < vector < int> >  aeps_in;
       
        for (int i = 0; i < lista_cfg.size(); i++) {
            vector < int > temp_aepsin;
            int it_cfg = lista_cfg[i]; 
            for (int j = 0; j < nos[it_cfg].size(); j++) {
                if ((nos[it_cfg][j].size()>0)&&(nos[it_cfg][j][0].in_acft>0)) {
                    //lp << " tama: " << nos[it_cfg][j].size() << endl;
                    temp_aepsin.push_back(nos[it_cfg][j][0].in_acft);
                    for ( int k = 0; k < matrics[it_cfg].size(); k++ ) { 
                        //lp << k << " " << matrics[it_cfg][k] << endl;
                        string varName = name_input(it_cfg, j, matrics[it_cfg][k]);
                        //lp << varName << endl;
                        string_bin += varName + " ";
                    }
                }
                else
                    temp_aepsin.push_back(0);
            }
            aeps_in.push_back(temp_aepsin);
        }  */
        
         string string_bin="";
        //lp << "test3" << endl;
        // define variavel de voo
        for (int p=0;p<lista_cfg.size();p++){
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){
                string flt_temp = cfgs[it_cfg].fltids[i];
                int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                if (flts[flt_pos].result >= 0)
                    //lp << "t2 " <<  matrics[it_cfg].size() << endl;
                    for ( int k = 0; k < matrics[p].size(); k++ ) {
                        string varName = name_voo(flts[flt_pos].cfg, flt_temp, flts[flt_pos].result, matrics[p][k]);
                        //lp <<  matrics[it_cfg][k] << " " << varName << endl;
                        string_bin += varName + " ";
                    }
            }    
        }        
        
        // define variavel de solo
        string string_int="";
        int it_cfg;
        //lp << "test4" << endl;
        
        for (int p=0;p<namesga.size();p++) {
            if (results_namesga[p]>0) {
                it_cfg = conf_namesga[p];
                auto it = find(lista_cfg.begin(),lista_cfg.end(),it_cfg);
                if (it!=lista_cfg.end()) {
                    int cnf_pos = distance(lista_cfg.begin(),it);
                    string varName = namesga[p];
                    for ( int k = 0; k < matrics[cnf_pos].size(); k++ )
                        string_int += varName + "_" + to_string(matrics[cnf_pos][k]) + " ";
                }
            }
        }
        
    
        lp << "Minimize" << endl;
        lp << "  ";
        
        vector < string >  string_rest;
        
        // cobertura variavel de voo
       
        string string_obj="";
        //lp << "test5" << endl;
        for (int p=0;p<lista_cfg.size();p++){
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){
                string flt_temp = cfgs[it_cfg].fltids[i];
                int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                if (flts[flt_pos].result >= 0) {
                    
                    //objective function
                    string tmpacft = flts[flt_pos].org_acft;
                    int acft_pos = distance(acfts.begin(),find_if(acfts.begin(),acfts.end(), [tmpacft] (const aircraft_struct& a) { return a.id==tmpacft; }));
                    
                    //couverture
                    string constraintName = "Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
                    string texto ="";
                    texto += " " + constraintName + ": ";
                    for ( int k = 0; k < matrics[p].size(); k++ ) {
                        
                            string varName = name_voo(flts[flt_pos].cfg, cfgs[it_cfg].fltids[i], flts[flt_pos].result, matrics[p][k]);
                            texto += " + " + varName ;
                            
                            if ( matrics[p][k] != acft_pos )
                                string_obj += " + " + varName;
                            
                    }
                    texto += " = 1";
                    string_rest.push_back(texto);
                    
                }    
            }    
        }        
        bool saida = false;
        //lp << "test2" << endl;
        // cobertura variavel de voo maintencance
         
        //p << "test5" << endl;
        for (int p=0;p<lista_cfg.size();p++){
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){
                string flt_temp = cfgs[it_cfg].fltids[i];
                int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                if (flts[flt_pos].result >= 0) {
                    
                    //objective function
                    string tmpacft = flts[flt_pos].org_acft;
                    int acft_pos = distance(acfts.begin(),find_if(acfts.begin(),acfts.end(), [tmpacft] (const aircraft_struct& a) { return a.id==tmpacft; }));
                    
                    //couverture
                    string constraintName = "Maintenance_Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
                    string texto ="";
                    texto += " " + constraintName + ": ";
                    for ( int k = 0; k < matrics[p].size(); k++ ) {
                        if ((flts[flt_pos].mant)&&( matrics[p][k] == acft_pos ))  {
                            string varName = name_voo(flts[flt_pos].cfg, cfgs[it_cfg].fltids[i], flts[flt_pos].result, matrics[p][k]);
                            texto += " + " + varName ;
                            texto += " = 1";
                            string_rest.push_back(texto);
                            saida = true;
                            break;
                        }
                    }
                }    
                //if (saida) break;
            }
            //if (saida) break;
        }        
        
        /* cobertura variavel de voo input
        saida = false;
        //lp << "test5" << endl;
        for (int p=0;p<lista_cfg.size();p++){
            int it_cfg = lista_cfg[p];
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){
                string flt_temp = cfgs[it_cfg].fltids[i];
                int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                if (flts[flt_pos].result >= 0) {
                    
                    //objective function
                    string tmpacft = flts[flt_pos].org_acft;
                    int acft_pos = distance(acfts.begin(),find_if(acfts.begin(),acfts.end(), [tmpacft] (const aircraft_struct& a) { return a.id==tmpacft; }));
                    
                    
                    
                    for ( int k = 0; k < matrics[p].size(); k++ ) {
                        if ((flts[flt_pos].inflight)&&( matrics[p][k] == acft_pos ))  {
                            
                            //couverture
                            string constraintName = "Input_Aircraft_Flight_Couverture_" + to_string(k) + "_" + flt_temp;
                            string texto ="";
                            texto += " " + constraintName + ": ";
                            string varName = name_voo(flts[flt_pos].cfg, cfgs[it_cfg].fltids[i], flts[flt_pos].result, matrics[p][k]);
                            texto += " + " + varName ;
                            texto += " = 1";
                            string_rest.push_back(texto);
                            saida = true;
                            break;
                        }
                    }
                }    
                //if (saida) break;
            }
            //if (saida) break;
        }      */  
        
        
        //lp << "test6" << endl;
        // cobertura variavel de solo
        for (int p=0;p<namesga.size();p++) {
            if (results_namesga[p]>0) {
                string texto ="";
                it_cfg = conf_namesga[p];
                auto it = find(lista_cfg.begin(),lista_cfg.end(),it_cfg);
                if (it!=lista_cfg.end()) {
                    string varName = namesga[p];
                    string constraintName = "GA_Couverture_" + to_string(it_cfg) + "_" + varName;
                    texto += " " + constraintName + ":";
                    int cnf_pos = distance(lista_cfg.begin(),it);
                    for ( int k = 0; k < matrics[cnf_pos].size(); k++ ) {
                        string arc_name = " + " + varName + "_" + to_string(matrics[cnf_pos][k]);
                        texto +=  arc_name ;
                    }
                    string busc = " = " + to_string(results_namesga[p]);
                    texto += busc;
                    string_rest.push_back(texto);            
                }
            }
        }
        
        //lp << "test7" << endl;
        /* cobertura nos de entrada
        for (int i = 0; i < lista_cfg.size(); i++) {
            int it_cfg = lista_cfg[i]; 
            for (int j = 0; j < nos[it_cfg].size(); j++) {
                if ((nos[it_cfg][j].size()>0)&&(nos[it_cfg][j][0].in_acft>0)) {
                    string texto = " " ;
                    string varName = to_string(it_cfg) + "_" + nos[it_cfg][j][0].local;
                    string constraintName =  " ";
                    constraintName += "Input_Couverture_" + varName + ":";
                    texto += constraintName;
                    for ( int k = 0; k < matrics[it_cfg].size(); k++ ) {
                        string varName2 = name_input (it_cfg, j, matrics[it_cfg][k]);
                        texto += " + " + varName2;
                        string aep_acft = acfts[matrics[it_cfg][k]].airp;
                        if (aep_acft==nos[it_cfg][j][0].local)
                            
                        
                        
                    }
                    texto += " = " + to_string(aeps_in[it_cfg][j]);
                    string_rest.push_back(texto); 
                    
                    
                    
                }
            }
        } */ 
        //lp << "test8" << endl;
        
        
        //lp << "test7" << endl;
        /* cobertura nos de entrada
        for (int i = 0; i < lista_cfg.size(); i++) {
            int it_cfg = lista_cfg[i]; 
            for (int j = 0; j < nos[it_cfg].size(); j++) {
                if ((nos[it_cfg][j].size()>0)&&(nos[it_cfg][j][0].in_acft>0)) {
                    for ( int k = 0; k < matrics[it_cfg].size(); k++ ) {
                        string texto = " " ;
                        string varName = to_string(it_cfg) + "_" + nos[it_cfg][j][0].local + "_" + to_string(k);
                        string constraintName =  " ";
                        constraintName += "Input_Couverture_" + varName + ":";
                        texto += constraintName;
                        string varName2 = name_input (it_cfg, j, matrics[it_cfg][k]);
                        string aep_acft = acfts[matrics[it_cfg][k]].airp;
                        if (aep_acft==nos[it_cfg][j][0].local)    
                            texto += " + " + varName2 + " = 1";
                        else
                            texto += " + " + varName2 + " = 0";
                        string_rest.push_back(texto); 
                    }
                }
            }
        }  */ 
        
        
        
        //Balance Flow Constraint  
        for (int p=0;p<lista_cfg.size();p++) {
            int it_cfg = lista_cfg[p];
            for (int i=0;i<nos[it_cfg].size();i++)
                for (int k=0;k<nos[it_cfg][i].size();k++)
                    if (nos[it_cfg][i][k].key.substr(0,3)!="OUT") 
                        for ( int mat = 0; mat < matrics[p].size(); mat++ ) {
                            string rest = BF_str(it_cfg, i, k, matrics[p][mat]);
                            if (rest.size())
                                string_rest.push_back(rest);
                        }
        }
        cout << "Balance Flow Constraint Added" << endl;
        
        //lp << "test9" << endl;         
        
        
	// Objective Function    
        lp << string_obj;
        lp << endl << "Subject To" << endl;
        for (int i=0;i<string_rest.size();i++) lp << string_rest[i] << endl;
        lp << "Bounds" << endl << "Binaries" << endl << string_bin << endl << "Generals" << endl << string_int << endl << "End" << endl;
        //lp << "test10" << endl;
	cout << "Objective function added" << endl;  
        lp.close();
        time_t now1 = time(0);
    
}

struct struct_balance_out {
    vector < string > arc;
    string result;
};

struct_balance_out verify_out (int it_cfg, int i, int k ){
   
    struct_balance_out tempout;
    string texto2;
    
    for (int j=0;j<nos[it_cfg][i][k].infltarc.size();j++) {
      string flt_temp = nos[it_cfg][i][k].infltarc[j];         
      if (arc_result(flt_temp)){
          string busc = "xfc_" + to_string(it_cfg) + "_" + flt_temp + "_";        
          texto2 =  " + " + busc;
          tempout.arc.push_back(texto2); 
      }
    }

    for (int j=0;j<nos[it_cfg][i][k].ingrdarc.size();j++) 
        if (result_ga_gas(it_cfg,nos[it_cfg][i][k].ingrdarc[j]) > 0) {
              string flt_temp = nos[it_cfg][i][k].ingrdarc[j] + "_" ;
              texto2 =  " + " + flt_temp;
              tempout.arc.push_back(texto2); 
        }
    

    for (int j=0;j<nos[it_cfg][i][k].outfltarc.size();j++) {
        string flt_temp = nos[it_cfg][i][k].outfltarc[j];
        if (arc_result(flt_temp)){
            string busc = "xfc_" + to_string(it_cfg) + "_" + flt_temp + "_" ; 
            texto2 =  " - " + busc;
            tempout.arc.push_back(texto2); 
        }
    }

    for (int j=0;j<nos[it_cfg][i][k].outgrdarc.size();j++) 
        if (result_ga_gas(it_cfg,nos[it_cfg][i][k].outgrdarc[j]) > 0) {
          string flt_temp = nos[it_cfg][i][k].outgrdarc[j] + "_" ;
          texto2 =  " - " + flt_temp;
          tempout.arc.push_back(texto2);
        }
    
    
    return tempout;
}

// ROUTING SEPARATED CONFIGURATIONS TEXT FILE
void create_lp_fifo_separada() {


        
        ofstream cout((char*)nome_output.c_str(), std::fstream::app);
        
        cout << "LP FIFO construction" << endl << endl;
        
        // cria vetor com as matriculas
        //lp << "test1" << endl;
        
     
        
        string string_int="";
        string string_bin="";
        vector < string >  string_rest;
        vector < string >  string_rest_mant;
        string string_obj="";
       
        string nome_arq;
        
        
        for (int i = 0; i < lista_cfg.size(); i++) {
            int id_cfg = lista_cfg[i];
            vector < int > temp_matrics;
            for (int j = 0; j < acfts.size(); j++)
                if (acfts[j].cfg == id_cfg)
                    temp_matrics.push_back(j);
            matrics.push_back(temp_matrics);
        }  
        
        
        
        //lp << "test3" << endl;
        // define variavel de voo
        for (int p=0;p<lista_cfg.size();p++){
            
          
            
            cout << "Config " << p << endl;
            int it_cfg = lista_cfg[p];
            nome_arq = nome_lp_fifo + "_" + cfgs[it_cfg].model + to_string(cfgs[it_cfg].ass) + ".lp";
            ofstream lp((char*)nome_arq.c_str());
            lps.push_back(nome_arq);
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){
                string flt_temp = cfgs[it_cfg].fltids[i];
                int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                if (flts[flt_pos].result >= 0)
                    //lp << "t2 " <<  matrics[it_cfg].size() << endl;
                    for ( int k = 0; k < matrics[p].size(); k++ ) {
                        string varName = name_voo(flts[flt_pos].cfg, flt_temp, flts[flt_pos].result, matrics[p][k]);
                        string_bin += varName + " ";
                    }
            }    
            
            cout << "Flight Variables Added " << endl;
            
            // define variavel de solo
            for ( int i=0; i < gas[it_cfg].size();i++) 
                for ( int k = 0; k < matrics[p].size(); k++ ) {
                    string varName = gas[it_cfg][i].name + "_" + to_string(matrics[p][k]) + " ";
                    string_bin += varName;
                    string_obj += " + " + varName;
                }
            
            cout << "Ground Variables and Objective Function Added " << endl;
            
            // cobertura variavel de voo

            
            //lp << "test5" << endl;
            
            for (int i=0;i<cfgs[it_cfg].fltids.size();i++){
                    string flt_temp = cfgs[it_cfg].fltids[i];
                    int flt_pos = distance(flts.begin(),find_if(flts.begin(),flts.end(), [flt_temp] (const flight_struct& a) { return a.flid==flt_temp; }));
                    if (flts[flt_pos].result >= 0) {

                        //objective function and couverture
                        string tmpacft = flts[flt_pos].org_acft;
                        int acft_pos = distance(acfts.begin(),find_if(acfts.begin(),acfts.end(), [tmpacft] (const aircraft_struct& a) { return a.id==tmpacft; }));
                        
                        if (!flts[flt_pos].mant) {
                             // flown flight couverture
                            string constraintName = "Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
                            string texto ="";
                            texto += " " + constraintName + ": ";
                            for ( int k = 0; k < matrics[p].size(); k++ ) {
                                    //objective function
                                    string varName = name_voo(flts[flt_pos].cfg, cfgs[it_cfg].fltids[i], flts[flt_pos].result, matrics[p][k]);
                                    texto += " + " + varName ;
                                    //if ( matrics[p][k] != acft_pos )
                                    //    string_obj += " + " + varName;
                            }
                            texto += " = 1";
                            string_rest.push_back(texto);
                        }
                        else{
                            
                             // maintenace couverture
                            string constraintName = "Maintenance_Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
                            string texto ="";
                            texto += " " + constraintName + ": ";
                            for ( int k = 0; k < matrics[p].size(); k++ ) {
                                if ( matrics[p][k] == acft_pos )  {
                                    string varName = name_voo(flts[flt_pos].cfg, cfgs[it_cfg].fltids[i], flts[flt_pos].result, matrics[p][k]);
                                    texto += " + " + varName ;
                                    texto += " = 1";
                                    string_rest_mant.push_back(texto);
                                    break;
                                }
                            }
                        }    
                        
                    }
            }    
            
            
            cout << "Flight and Maintenance Couverture Restrictions Added " << endl;
            
        
        
            // cobertura arco de solo
            for (int i=0;i<gas[it_cfg].size();i++) {
                    string texto ="";
                        string varName = gas[it_cfg][i].name;
                        string constraintName = "GA_Couverture_" + to_string(it_cfg) + "_" + varName;
                        texto += " " + constraintName + ":";
                        //int cnf_pos = distance(lista_cfg.begin(),it);
                        for ( int k = 0; k < matrics[p].size(); k++ ) {
                            string arc_name = " + " + varName + "_" + to_string(matrics[p][k]);
                            texto +=  arc_name ;
                        }
                        string busc = " = " + to_string(gas[it_cfg][i].result);
                        texto += busc;
                        string_rest.push_back(texto);            
            }
            cout << "Ground Couverture Added " << endl;
        
        
            //Balance Flow Constraint  
            for (int i=0;i<nos[it_cfg].size();i++)
                    for (int k=0;k<nos[it_cfg][i].size();k++)
                        if ((nos[it_cfg][i][k].key.substr(0,3)!="OUT")&&(nos[it_cfg][i][k].tout_out)) {
                            
                            struct_balance_out res = verify_out(it_cfg, i, k);
                            
                            
                            if (res.arc.size()>0)
                                for ( int mat = 0; mat < matrics[p].size(); mat++ ) {
                                    string res_bal = "";
                                    int aern = matrics[p][mat];
                                    string constraintName = "BF_" + to_string(it_cfg) + "_" + nos[it_cfg][i][k].key + "_" + to_string(aern);
                                    res_bal += " " + constraintName + ": ";
                                    for (int j=0;j<res.arc.size();j++)
                                        res_bal += res.arc[j] + to_string(aern);
                                    if ((acfts[aern].airp==nos[it_cfg][i][k].local)&&(k==0))    
                                        res.result = " = - 1";
                                    else
                                        res.result = " = 0";
                                    res_bal += res.result;
                                    string_rest.push_back(res_bal); 
                                }
                                cout << "BF restriction " << it_cfg << " " << i << " " << k << " Added" << endl;
                        }    

            string_rest.insert(string_rest.end(),string_rest_mant.begin(),string_rest_mant.end());
            cout << "Balance Flow Constraint Added" << endl;

            //lp << "test9" << endl;         
            lp << "\\ Routing Linear Programming " << nome_instancia << endl;
            lp << "Minimize" << endl;
            lp << "  ";    
            lp << string_obj;
            lp << endl << "Subject To" << endl;
            for (int i=0;i<string_rest.size();i++) lp << string_rest[i] << endl;
            lp << "Bounds" << endl << "Binaries" << endl << string_bin << endl << "End" << endl;
            lp.close();
            //lp << "test10" << endl;
           // cout << "Objective function added" << endl;
            string_int="";
            string_bin="";
            string_rest.clear();
            string_rest_mant.clear();
            string_obj="";
        }
       
        
        
    
        time_t now1 = time(0);
    
}


int vecgbxfc2(int buscado, int opc, int nacfts, int posacft) {
	int qtd = nacfts;
	int posflid = flts[buscado].inic_xfc;
	int mpos1 = qtd * opc + posflid + posacft;
	return mpos1;
}


int vecgbyf(string buscado) {
	return flts[flidtoflpos(buscado)].inic_yf;
}

int vecgbzga(int pos, int posacft) {
	return inic_gbga[pos] + posacft;
}

// FLEET AND ROUTING INTEGRATED GUROBI
rodada_struct create_lp_routing_assignment_gurobi2(string nome_lp2, vector <int> lista_cfg_p) {

	//ofstream lp((char*)nome_lp.c_str());
	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	ofstream res_file_gnr((char *)arqresgeral.c_str(), std::fstream::app);

	rodada_struct tempexact;

	vector <int> lista_cfg2;
	for (int i = 0; i<cfgs.size(); i++)
		if (cfgs[i].model != "TranspCom")
			lista_cfg2.push_back(i);


	try {
		GRBEnv *env = NULL; // creates the GUROBI environment
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);    // creates an empty model
		

		GRBVar *yf = NULL;
		yf = new GRBVar[flts.size()]; //cancel decision variable


		int max_nflcfg = -1;
		for (int i = 0; i < lista_cfg2.size(); i++) {
			int id_cfg = lista_cfg2[i];
			int nflcfg = count_if(acfts.begin(), acfts.end(), [id_cfg](const aircraft_struct& a) { return a.cfg == id_cfg; });
			if (nflcfg > max_nflcfg)
				max_nflcfg = nflcfg;
		}

		GRBVar *xfc = NULL;
		xfc = new GRBVar[flts.size()*noptions*max_nflcfg]; //flight decision variable

		GRBVar *zga = NULL;
		zga = new GRBVar[namesga.size()*max_nflcfg]; //ground arc decision variable       

		model.update();

		matrics.clear();
		for (int i = 0; i < lista_cfg2.size(); i++) {
			int id_cfg = lista_cfg2[i];
			vector < int > temp_matrics;
			for (int j = 0; j < acfts.size(); j++)
				if (acfts[j].cfg == id_cfg) 
					temp_matrics.push_back(j);
			matrics.push_back(temp_matrics);
		}


		cout << "LP Routing Assignment" << endl << endl;
		GRBLinExpr expr_obj = 0;
		string string_bin = "";
		int nflights = 0;
		for (int i = 0; i < flts.size(); i++)
			if (find(lista_cfg2.begin(), lista_cfg2.end(), flts[i].cfg) != lista_cfg2.end()) {
				if (!flts[i].mant) {
					string varName = "yf_" + to_string(flts[i].cfg) + "_" + flts[i].flid;
					string_bin += varName + " ";
					double cc;
					if (find(lista_cfg_p.begin(), lista_cfg_p.end(), flts[i].cfg) != lista_cfg_p.end())
						cc = cancel_cost;
					else
						cc = 0.0;
					yf[nflights] = model.addVar(0.0, 1.0, cc, GRB_BINARY, varName);
					flts[i].inic_yf = nflights;
					//expr_obj += yf[nflights] * cancel_cost;
					nflights++;
				}
			}
		


		cout << "yf variables added" << endl;

		int h = 0;
		int init_delay = 0;

		for (int p = 0; p < lista_cfg2.size(); p++) {
			int it_cfg = lista_cfg2[p];
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
				flts[flt_pos].inic_xfc = h;
				string acf = flts[flt_pos].org_acft;
				int acftpos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [acf](const aircraft_struct& a) { return a.id == acf; }));
				for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
					for (int k = 0; k < matrics[p].size(); k++) {
						string varName = "xfc_" + to_string(flts[flt_pos].cfg) + "_" + flts[flt_pos].kfs[j] + "_" + to_string(matrics[p][k]);
						//string_bin += varName + " ";
						int changecost = 0;
						if ((matrics[p][k] != acftpos)&&(!flts[flt_pos].mant))
							changecost = 1;
						if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end())
							xfc[h] = model.addVar(0.0, 1.0, minute_cost * min_opt * j + changecost, GRB_BINARY, varName);
						else
							xfc[h] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
						h++;
					}
				}
			}
		}
		cout << "xfc variables added" << endl;


		h = 0;
		inic_gbga.clear();



		for (int p = 0; p < namesga.size(); p++) {
			auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
			if (it != lista_cfg2.end()) {
				int cnfmat = distance(lista_cfg2.begin(), it);
				inic_gbga.push_back(h);
				for (int k = 0; k < matrics[cnfmat].size(); k++) {
					string varName = namesga[p] + "_" + to_string(matrics[cnfmat][k]);
					zga[h] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
					//cout << p << " " << conf_namesga[p] << " " << cnfmat  << " " << matrics[cnfmat].size() << " " << h << " " << varName << endl;
					h++;
				}
			}
		}
		cout << "zga variables added" << endl;




		model.update();
		cout << endl << "Lista de Configuracoes: ";
		for (size_t i = 0; i < lista_cfg_p.size(); i++)
			cout << lista_cfg_p[i] << " ";
		cout << endl;


		//Balance Flow Constraint  
		vector < string >  string_rest;
		string text_rest("");
		for (int p = 0; p < lista_cfg2.size(); p++) {
			int it_cfg = lista_cfg2[p];
			if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end()) {
				for (int i = 0; i < nos[it_cfg].size(); i++)
					for (int k = 0; k < nos[it_cfg][i].size(); k++) {
						if (nos[it_cfg][i][k].key.substr(0, 3) != "OUT") {

							for (int mat = 0; mat < matrics[p].size(); mat++) {

								GRBLinExpr expr = 0;
								string constraintName = "BF_" + to_string(it_cfg) + "_" + nos[it_cfg][i][k].key + "_" + to_string(matrics[p][mat]);

								for (int j = 0; j < nos[it_cfg][i][k].infltarc.size(); j++) {
									int flt_temp = nos[it_cfg][i][k].infltarc_int[j].flidpos;
									int opc = nos[it_cfg][i][k].infltarc_int[j].opc;
									int gbpos = vecgbxfc2(flt_temp, opc, matrics[p].size(), mat);
									expr += xfc[gbpos];
								}
								for (int j = 0; j < nos[it_cfg][i][k].ingrdarc.size(); j++) {
									int flt_pos = nos[it_cfg][i][k].ingrdarc_int[j];
									int gbpos = vecgbzga(flt_pos, mat);
									expr += zga[gbpos];
								}
								for (int j = 0; j < nos[it_cfg][i][k].outfltarc.size(); j++) {
									int flt_temp = nos[it_cfg][i][k].outfltarc_int[j].flidpos;
									int opc = nos[it_cfg][i][k].outfltarc_int[j].opc;
									int gbpos = vecgbxfc2(flt_temp, opc, matrics[p].size(), mat);
									expr -= xfc[gbpos];
								}

								for (int j = 0; j < nos[it_cfg][i][k].outgrdarc.size(); j++) {
									int flt_pos = nos[it_cfg][i][k].outgrdarc_int[j];
									int gbpos = vecgbzga(flt_pos, mat);
									expr -= zga[gbpos];
								}
								int aern = matrics[p][mat];
								string res_str;
								int res_int;
								if ((acfts[aern].airp == nos[it_cfg][i][k].local) && (k == 0)) {
									res_str = " = - 1";
									res_int = -1;
								}
								else {
									res_str = " = 0";
									res_int = 0;
								}
								
								if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end()) {
									//cout << " " << constraintName << " " << expr << endl;
									model.addConstr(expr == res_int, constraintName);
								}

							}
						}
					}
			}
		}
		cout << "Balance Flow Constraint Added" << endl;


		//Cancel or Delay Constraints
		for (int p = 0; p < lista_cfg_p.size(); p++) {
			int it_cfg = lista_cfg_p[p];
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				//cout << flt_temp << endl;
				int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
				if (!mant_flid(flt_temp)) {
					GRBLinExpr expr = 0;
					string texto = "";
					string constraintName = "CancelDelay_" + to_string(it_cfg) + "_" + flt_temp;
					string varName = "yf_" + to_string(it_cfg) + "_" + flt_temp;
					texto += " " + constraintName + ": " + varName;
					string buscado = flt_temp;
					int gbpos = vecgbyf(buscado);
					expr += yf[gbpos];
					int cnfmat = distance(lista_cfg2.begin(), find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg));
					for (int mat = 0; mat < matrics[cnfmat].size(); mat++) {
						string textkfs("");
						for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
							string fl_temp = flts[flt_pos].kfs[j];
							int gbpos2 = vecgbxfc2(arctoflpos(fl_temp), arctoopc(fl_temp), matrics[cnfmat].size(), mat);
							expr += xfc[gbpos2];
						}
					}
					//cout << expr << endl;
					//if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end())
						model.addConstr(expr == 1, constraintName);
				}
				else
					if (!flid_alt_acft(flt_temp)) {
						GRBLinExpr expr = 0;
						string tmpacft = flts[flt_pos].org_acft;
						int acft_pos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [tmpacft](const aircraft_struct& a) { return a.id == tmpacft; }));
						// maintenace couverture
						string constraintName = "Maintenance_Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
						int cnfmat = distance(lista_cfg2.begin(), find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg));
						for (int k = 0; k < matrics[cnfmat].size(); k++) {
							if (matrics[cnfmat][k] == acft_pos) {
								int gbpos2 = vecgbxfc2(flt_pos, 0, matrics[cnfmat].size(), k);
								expr += xfc[gbpos2];
								//cout << expr << endl;
								//if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end())
									model.addConstr(expr == 1, constraintName);
								break;
							}
						}
					}

			}
		}
		cout << "Coverture - Cancel or Delay Constraints" << endl;

		// cobertura arco de solo


		//Slot Capacity Constraint
		for (int i = 0; i < evts.size(); i++) {
			if (evts[i].key_flight != "END_REVOVERY") {
				int evt_cnf = evts[i].cnfg;
				auto it = find(lista_cfg_p.begin(), lista_cfg_p.end(), evt_cnf);
				if (!mant_arc(evts[i].key_flight) && (it != lista_cfg_p.end())) {
					//cout << "test1" << endl;
					slot_struct temp_slot;
					string tp;
					int tipo_cap_slot;
					if (evts[i].type) {
						temp_slot.tipo = "d";
						tipo_cap_slot = 0;
					}
					else {
						temp_slot.tipo = "a";
						tipo_cap_slot = 1;
					}
					temp_slot.key_slot = evts[i].local + to_string(evts[i].h_slot) + temp_slot.tipo;
					int h_busca = evts[i].h_slot % 24;
					string aep_temp = evts[i].local;
					int aep = distance(aerps.begin(), find_if(aerps.begin(), aerps.end(), [aep_temp](const airport_struct& a) { return a.nome == aep_temp; }));
					//temp_slot.rest = aerps[aep].slot_lim[h_busca][tipo_cap_slot];
					//cout << "test3" << endl;
					//cout << evts[i].h_slot;
					if (tipo_cap_slot == 0)
						temp_slot.rest = airpcap[aep].depcap[evts[i].h_slot];
					else
						temp_slot.rest = airpcap[aep].arrvcap[evts[i].h_slot];

					//cout << " adicionada" << endl;

					int p = distance(lista_cfg2.begin(), find(lista_cfg2.begin(), lista_cfg2.end(), evt_cnf));
					//cout << "test4" << endl;
					string busc("");
					GRBLinExpr expr = 0;
					int cont = 0;
					for (int mat = 0; mat < matrics[p].size(); mat++) {
						int gbpos = vecgbxfc2(arctoflpos(evts[i].key_flight), arctoopc(evts[i].key_flight), matrics[p].size(), mat);
						//if (find(lista_cfg_p.begin(), lista_cfg_p.end(), evt_cnf) != lista_cfg_p.end()) {
							expr += xfc[gbpos];
							cont++;
						//}
						//else
						//	expr += 0.0 * xfc[gbpos];
					}
					//cout << "test5" << endl;
					temp_slot.name = "Slot_" + temp_slot.key_slot;
					temp_slot.expr = expr;
					temp_slot.constraint = "Slot_" + temp_slot.key_slot + " : " + busc;
					temp_slot.cont = cont;
					string key_temp = temp_slot.key_slot;
					auto it = find_if(slotconsts.begin(), slotconsts.end(), [key_temp](const slot_struct& a) { return a.key_slot == key_temp; });
					//cout << "test6" << endl;
					if (it == slotconsts.end())
							slotconsts.push_back(temp_slot);
					else {
						int pos = distance(slotconsts.begin(), it);
						slotconsts[pos].constraint += busc;
						slotconsts[pos].name = temp_slot.name;
						slotconsts[pos].expr += temp_slot.expr;
						slotconsts[pos].cont += temp_slot.cont;
					}
					// cout << "test7" << endl;
				}
			}
		}
		cout << "Slot Capacity Constraint" << endl;

		int nhacft = 0;
		if (alt_acfts) {
			for (int i = 0; i < altacft.size(); i++) {
				int it_cfg = altacft[i].cfg;
				auto it = find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg);
				if (it != lista_cfg_p.end()) {
					GRBLinExpr expr = 0;
					int cnfmat = distance(lista_cfg2.begin(), find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg));
					string texto = "";
					string constraintName = "Acft_unavailabe_" + to_string(it_cfg) + "_" + to_string(i);
					string buscado = altacft[i].acft_id;
					int matacfts = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [buscado](const aircraft_struct& a) { return a.id == buscado; }));
					for (int k = 0; k < matrics[cnfmat].size(); k++) {
						if (matrics[cnfmat][k]== matacfts)
							for (int j = 0; j < altacft[i].flids.size(); j++) {
								int gbpos = vecgbxfc2(flidtoflpos(altacft[i].flids[j]), 0, matrics[cnfmat].size(), k);
								expr += xfc[gbpos];
							}
					}
					//if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end()) {
						model.addConstr(expr == 1, constraintName);
						nhacft += (altacft[i].t2 - altacft[i].t1) / 60;
					//}
				}
			}
		}
		cout << "Aircraft Unavailable Constraint" << endl;
		text_rest = "";
		if (alt_flights) {
			for (int i = 0; i < altflts.size(); i++) {
				int it_cfg = conf_flid(altflts[i].flid);
				auto it = find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg);
				if (it != lista_cfg_p.end()) {
					GRBLinExpr expr = 0;
					int res_gb;
					int cnfmat = distance(lista_cfg2.begin(), find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg));
					string flt_temp = altflts[i].flid;
					string texto = "";
					int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
					string constraintName = "Altflight_" + to_string(flts[flt_pos].cfg) + "_" + altflts[i].flid;
					if (altflts[i].atr > 0) {
						for (int k = 0; k < matrics[cnfmat].size(); k++) {
							for (int j = 0; j < altflts[i].atr_int; j++) {
								int gbpos = vecgbxfc2(flidtoflpos(altflts[i].flid), j, matrics[cnfmat].size(), k);
								expr += xfc[gbpos];
							}
						}
						res_gb = 0;
					}
					else {
						texto += "yf_" + to_string(flts[flt_pos].cfg) + "_" + altflts[i].flid;
						texto += " = 1";
						string buscado = altflts[i].flid;
						int gbpos = vecgbyf(buscado);
						expr += yf[gbpos];
						res_gb = 1;
					}
					//if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end())
						model.addConstr(expr == res_gb, constraintName);
				}
			}
		}

		int ncapred = 0; int n15 = 0; int n30 = 0; int n60 = 0; int nm60 = 0; int nswap = 0; int ncancpos = 0;
		cout << "Flight perturbations Constraint" << endl;

		if (alt_arps)
			for (int i = 0; i < altslots.size(); i++) {
				for (int horaslot = altslots[i].h1; horaslot < altslots[i].h2; horaslot++) {
					// departure
					string keyslot = altslots[i].airp + to_string(horaslot) + "d";
					auto its = find_if(slotconsts.begin(), slotconsts.end(), [keyslot](const slot_struct& a) { return a.key_slot == keyslot; });
					if (its != slotconsts.end()) {
						int slot_pos = distance(slotconsts.begin(), its);

						//slotconsts[slot_pos].rest = altslots[i].dep;
						slotconsts[slot_pos].constraint = "Alt_" + slotconsts[slot_pos].constraint;
						slotconsts[slot_pos].name = "Alt_" + slotconsts[slot_pos].name;
					}
					// arrival
					keyslot = altslots[i].airp + to_string(horaslot) + "a";
					its = find_if(slotconsts.begin(), slotconsts.end(), [keyslot](const slot_struct& a) { return a.key_slot == keyslot; });
					if (its != slotconsts.end()) {
						int slot_pos = distance(slotconsts.begin(), its);
						//slotconsts[slot_pos].rest = altslots[i].arrv;
						slotconsts[slot_pos].constraint = "Alt_" + slotconsts[slot_pos].constraint;
						slotconsts[slot_pos].name = "Alt_" + slotconsts[slot_pos].name;
					}
					ncapred++;
				}
				
			}

		for (int i = 0; i < slotconsts.size(); i++) {
			//string_rest.push_back(slotconsts[i].constraint + " <= " + to_string(slotconsts[i].rest));
			if (slotconsts[i].cont>0)
				model.addConstr(slotconsts[i].expr <= slotconsts[i].rest, slotconsts[i].name);
		}
		cout << "Airport Perturbations Constraint" << endl;


		// Calculate non positioned airport
		int dif_pos = 0;
		for (int p = 0; p < lista_cfg_p.size(); p++) {
			int it_cfg = lista_cfg_p[p];
			for (int i = 0; i < nos[it_cfg].size(); i++) {
				if (nos[it_cfg][i].size() > 0) {
					int orgpos = 0;
					if (aerps[i].acft_pos.size() > 0)
						orgpos = aerps[i].acft_pos[it_cfg];
					int timeend = recovery_time.end + cfgs[it_cfg].tat;
					int posrcv = distance(nos[it_cfg][i].begin(), find_if(nos[it_cfg][i].begin(), nos[it_cfg][i].end(), [timeend](const activity_struct& a) { return a.timee == timeend; }));
					int realpos = nos[it_cfg][i][posrcv].outgrdarc_int[0];
					GRBLinExpr expr = 0;
					string constraintName = "PosAcfts_" + to_string(it_cfg) + "_" + aerps[i].nome;
					int cnfmat = distance(lista_cfg2.begin(), find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg));
					for (int k = 0; k < matrics[cnfmat].size(); k++) {
						int gbpos = vecgbzga(realpos, k);
						expr += zga[gbpos];
					}
					//if (find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end())
					model.addConstr(expr == orgpos, constraintName);
				}
			}
		}
		cout << "ACFT Positioned Airport Constraint" << endl;


		cout << "All Coinstraints " << endl;
		model.update();

		cout  << endl << endl;
		model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
		model.update();
		model.write(nome_lp2);
		model.getEnv().set(GRB_DoubleParam_MIPGap, 0);
		model.set(GRB_DoubleParam_TimeLimit, max_time);
		//cout << "test1" << endl;
		model.optimize();
		//cout << "test2" << endl;

		//string nome_result_assign = "./Results/" + nome_instancia + "/" + nome_instancia + "_" + to_string(noptions) + ".sol";
		//model.write(nome_result_assign);
		res_file_gnr << endl << "Exato  Configuracoes: ";
		for (size_t i = 0; i < lista_cfg_p.size(); i++)
			res_file_gnr << cfgs[lista_cfg_p[i]].model + to_string(cfgs[lista_cfg_p[i]].ass) << " ";
		
		int nflcfg = 0;
		for (size_t ni = 0; ni < acfts.size(); ni++)
			if (find(lista_cfg_p.begin(), lista_cfg_p.end(), acfts[ni].cfg) != lista_cfg_p.end())
				nflcfg++;

		
		res_file_gnr << endl << "Nome: " << nome_instancia << endl << "NACFTS: " << nflcfg << endl;
		int feasibility = 0;
		double gap1 = 0;
		int optimstatus;
		double result = -99;
		double result2 = 0;
		double timerun = 0;
		int ncanc = 0;
		

		if (model.get(GRB_IntAttr_SolCount) == 0) {
			//cout << "No solution found, optimization status = " << model.get(GRB_IntAttr_Status) << endl;
			feasibility = 0;
		}
		else {
			feasibility = 1;
			//gap1 = model.get(GRB_DoubleParam_MIPGap);
			gap1 = abs(model.get(GRB_DoubleAttr_ObjBound)- model.get(GRB_DoubleAttr_ObjVal))/ abs(model.get(GRB_DoubleAttr_ObjVal));
			result = model.get(GRB_DoubleAttr_ObjVal);
			timerun = model.get(GRB_DoubleAttr_Runtime);
			optimstatus = model.get(GRB_IntAttr_Status);

			//string nome_lp_fifo2_sol = nome_lp_fifo + ".sol";
			//model.write(nome_lp_fifo2_sol);

			//result = model.get(GRB);
			//cout << "Resultado: " << result << endl << "GAP: " << gap1 << endl;
			nflights = 0;
			int nflight_p = 0;
			for (int i = 0; i < flts.size(); i++)
				if (find(lista_cfg2.begin(), lista_cfg2.end(), flts[i].cfg) != lista_cfg2.end()) {
					if (!flts[i].mant) {
						if (find(lista_cfg_p.begin(), lista_cfg_p.end(), flts[i].cfg) != lista_cfg_p.end()) {
							if (yf[nflights].get(GRB_DoubleAttr_X) > 0.0001) {
								flts[i].result = -1;
								flts[i].cost = cancel_cost;
								ncancpos++;
							}
							if (flts[i].atr > 0)
								init_delay += flts[i].atr;
							if (flts[i].atr < 0)
								ncanc++;
							nflight_p++;
						}
						nflights++;
					}
				}


			h = 0;
			for (int p = 0; p < lista_cfg2.size(); p++) {
				int it_cfg = lista_cfg2[p];
				for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
					string flt_temp = cfgs[it_cfg].fltids[i];
					int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
					flts[flt_pos].inic_xfc = h;
					for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
						for (int k = 0; k < matrics[p].size(); k++) {
							if ((xfc[h].get(GRB_DoubleAttr_X) > 0.0001)&&(find(lista_cfg_p.begin(), lista_cfg_p.end(), it_cfg) != lista_cfg_p.end())) {
								flts[flt_pos].result = arctoopc(flts[flt_pos].kfs[j]);;
								flts[flt_pos].final_acft = acfts[matrics[p][k]].id;
								flts[flt_pos].cost = minute_cost * min_opt * arctoopc(flts[flt_pos].kfs[j]);
								if (flts[flt_pos].result == 1) n15++;
								if ((flts[flt_pos].result == 2)|| (flts[flt_pos].result == 3)) n30++;
								if (flts[flt_pos].result == 4) n60++;
								if (flts[flt_pos].result > 4) nm60++;
								if (flts[flt_pos].final_acft != flts[flt_pos].org_acft) nswap++;
							}
							h++;
						}
					}
				}
			}

			

			h = 0;
			for (int p = 0; p < namesga.size(); p++) {
				auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
				if (it != lista_cfg2.end()) {
					if (find(lista_cfg_p.begin(), lista_cfg_p.end(), conf_namesga[p]) != lista_cfg_p.end())
						results_namesga[p] = 0;
					int cnfmat = distance(lista_cfg2.begin(), it);
					inic_gbga.push_back(h);
					for (int k = 0; k < matrics[cnfmat].size(); k++) {
						if (zga[h].get(GRB_DoubleAttr_X) > 0.0001) 
							results_namesga[p] += 1;
						// do not store the aircrafts of each ground arc
						h++;
					}
				}
			}

			if (pos_rest) {
				cout << endl << "Calculate INTEGRATED non positioned airport" << endl;
				// Calculate non positioned airport
				int dif_pos = 0;
				for (int p = 0; p < lista_cfg_p.size(); p++) {
					int it_cfg = lista_cfg_p[p];
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
			}


			res_file_gnr << "NFLIGHT: " << nflight_p << endl << "INIT_FLIGHT_DELAY: " << init_delay << endl;
			res_file_gnr << "NCANC: " << ncanc << endl << "N_HOURS_AIRPORT_CAPACITY_REDUCTION: " << ncapred << endl << "N_HOURS_ACFT_BROKEN_PERIOD: " << nhacft << endl;
			res_file_gnr << "FEASIBLE: " << feasibility << endl << "RESULT: " << result << endl << "GAP: " << gap1 << endl;
			res_file_gnr << "Time: " << timerun << endl << "Status: " << optimstatus << endl << "NCANC_POS: " << ncancpos << endl << "NATZ15: " << n15 << endl;
			res_file_gnr << "NATZ30: " << n30 << endl << "NATZ60: " << n60 << endl << "NON POSITIONED AIRCFT: " << dif_pos << endl;
			res_file_gnr << "NATZM60: " << nm60 << endl;

			tempexact.NATZ15 = n15;
			tempexact.NATZ30 = n30;
			tempexact.NATZ60 = n60;
			tempexact.NATZM60 = nm60;
			tempexact.NCANC_POS = ncancpos;
			tempexact.NSWAPS = nswap;
			tempexact.dif_pos = dif_pos;

		}
		tempexact.NFLIGHT = nflights;
		tempexact.INIT_FLIGHT_DELAY = init_delay;
		tempexact.NCANC = ncanc;
		tempexact.N_HOURS_AIRPORT_CAPACITY_REDUCTION = ncapred;
		tempexact.N_HOURS_ACFT_BROKEN_PERIOD = nhacft;
		tempexact.FEASIBLE = feasibility;
		tempexact.gnr_feas = feasibility;
		tempexact.GAP = gap1;
		tempexact.Time = timerun;
		tempexact.Status = optimstatus;
		tempexact.TOTAL_RESULT = result;
		tempexact.RESULT_ASSIGN = result;


		cout << "Objective function added" << endl;
		time_t now1 = time(0);
		model.reset();
	}
	catch (GRBException e)
	{
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...)
	{
		cout << "Exception during optimization" << endl;
	}
	//lp.close();
	res_file_gnr.close();
	return tempexact;
}

// FLEET ASSIGNMENT GUROBI
rodada_struct create_lp_assignment_gurobi() {

	//ofstream lp((char*)nome_lp.c_str());
	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	ofstream res_file_gnr((char *)arqresgeral.c_str(), std::fstream::app);
	rodada_struct tempassign;

	int feasibility = 0;
	double gap1 = 0;
	int optimstatus;
	double result = -99;
	double result2 = 0;
	double timerun = 0;
	int ncanc = 0;

	vector <int> lista_cfg2;
	for (int i=0;i<cfgs.size();i++)
	  if (cfgs[i].model!="TranspCom")
	    lista_cfg2.push_back(i);

	try {
		GRBEnv *env = NULL; // creates the GUROBI environment
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);    // creates an empty model

		GRBVar *yf = NULL;
		yf = new GRBVar[flts.size()]; //cancel decision variable

		GRBVar *xfc = NULL;
		xfc = new GRBVar[flts.size()*noptions]; //flight decision variable

		GRBVar *zga = NULL;
		zga = new GRBVar[namesga.size()]; //ground arc decision variable       

		model.update();

		cout << "LP fleet Assignment" << endl << endl;
		GRBLinExpr expr_obj = 0;
		string string_bin = "";
		int nflights = 0;
		for (int i = 0; i < flts.size(); i++)
			if (find(lista_cfg2.begin(), lista_cfg2.end(), flts[i].cfg) != lista_cfg2.end()) {
				if (!flts[i].mant) {
					string varName = "yf_" + to_string(flts[i].cfg) + "_" + flts[i].flid;
					yf[nflights] = model.addVar(0.0, 1.0, cancel_cost, GRB_BINARY, varName);
					flts[i].inic_yf = nflights;
					nflights++;
				}
			}
		cout << "yf variables added" << endl;

		int h = 0;
		int init_delay = 0;

		for (int p = 0; p < lista_cfg2.size(); p++) {
			int it_cfg = lista_cfg2[p];
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				int flt_pos = flidtoflpos(flt_temp);
				flts[flt_pos].inic_xfc = h;
				for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
						string varName = "xfc_" + to_string(it_cfg) + "_" + flts[flt_pos].kfs[j];
						if (find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg) != lista_cfg2.end())
							xfc[h] = model.addVar(0.0, 1.0, minute_cost * min_opt * j, GRB_BINARY, varName);
						else
							xfc[h] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
						h++;
				}
			}
		}
		cout << "xfc variables added" << endl;
		h = 0;
		for (int p = 0; p < namesga.size(); p++) {
			auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
			if (it != lista_cfg2.end()) {
				int cnfmat = distance(lista_cfg2.begin(), it);
				inic_gbga.push_back(h);
				string varName = namesga[p];
				//cout << varName << " " << h << endl;
				zga[h] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, varName);
				h++;
			}
		}
		cout << "zga variables added" << endl;

		model.update();
		//Balance Flow Constraint  
		for (int p = 0; p < lista_cfg2.size(); p++) {
			int it_cfg = lista_cfg2[p];
			for (int i = 0; i < nos[it_cfg].size(); i++)
				for (int k = 0; k < nos[it_cfg][i].size(); k++) {
					if (nos[it_cfg][i][k].key.substr(0, 3) != "OUT") {

							GRBLinExpr expr = 0;
							string constraintName = "BF_" + to_string(it_cfg) + "_" + nos[it_cfg][i][k].key;

							for (int j = 0; j < nos[it_cfg][i][k].infltarc.size(); j++) {
								int flt_temp = nos[it_cfg][i][k].infltarc_int[j].flidpos;
								int opc = nos[it_cfg][i][k].infltarc_int[j].opc;
								int gbpos = vecgbxfc2(flt_temp, opc, 1, 0);
								expr += xfc[gbpos];
							}
							for (int j = 0; j < nos[it_cfg][i][k].ingrdarc.size(); j++) {
								int flt_pos = nos[it_cfg][i][k].ingrdarc_int[j];
								int gbpos = vecgbzga(flt_pos, 0);
								expr += zga[gbpos];
							}
							for (int j = 0; j < nos[it_cfg][i][k].outfltarc.size(); j++) {
								int flt_temp = nos[it_cfg][i][k].outfltarc_int[j].flidpos;
								int opc = nos[it_cfg][i][k].outfltarc_int[j].opc;
								int gbpos = vecgbxfc2(flt_temp, opc, 1, 0);
								expr -= xfc[gbpos];
							}
							for (int j = 0; j < nos[it_cfg][i][k].outgrdarc.size(); j++) {
								int flt_pos = nos[it_cfg][i][k].outgrdarc_int[j];
								int gbpos = vecgbzga(flt_pos, 0);
								expr -= zga[gbpos];
							}
							int res_int;
							if (k == 0) {
								res_int = - nos[it_cfg][i][k].in_acft;
							}
							else {
								res_int = 0;
							}
							model.addConstr(expr == res_int, constraintName);
					}
				}
		}
		cout << "Balance Flow Constraint Added" << endl;
		

		//Cancel or Delay Constraints
		for (int p = 0; p < lista_cfg2.size(); p++) {
			int it_cfg = lista_cfg2[p];
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
				if (!mant_flid(flt_temp)) {
					GRBLinExpr expr = 0;
					string texto = "";
					string constraintName = "CancelDelay_" + to_string(it_cfg) + "_" + flt_temp;
					string varName = "yf_" + to_string(it_cfg) + "_" + flt_temp;
					texto += " " + constraintName + ": " + varName;
					string buscado = flt_temp;
					int gbpos = vecgbyf(buscado);
					expr += yf[gbpos];
						string textkfs("");
						for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
							string fl_temp = flts[flt_pos].kfs[j];
							int gbpos2 = vecgbxfc2(arctoflpos(fl_temp), arctoopc(fl_temp), 1, 0);
							expr += xfc[gbpos2];
						}
					model.addConstr(expr == 1, constraintName);
				}
				else
					if (!flid_alt_acft(flt_temp)) {
						GRBLinExpr expr = 0;
						string tmpacft = flts[flt_pos].org_acft;
						int acft_pos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [tmpacft](const aircraft_struct& a) { return a.id == tmpacft; }));
						// maintenace couverture
						string constraintName = "Maintenance_Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
								int gbpos2 = vecgbxfc2(flt_pos, 0, 1, 0);
								expr += xfc[gbpos2];
								model.addConstr(expr == 1, constraintName);
					}

			}
		}
		cout << "Coverture - Cancel or Delay Constraints" << endl;
		
		//Slot Capacity Constraint
		for (int i = 0; i < evts.size(); i++) {
			if (evts[i].key_flight != "END_REVOVERY") {
				int evt_cnf = evts[i].cnfg;
				auto it = find(lista_cfg2.begin(), lista_cfg2.end(), evt_cnf);
				if (!mant_arc(evts[i].key_flight) && (it != lista_cfg2.end())) {
					slot_struct temp_slot;
					string tp;
					int tipo_cap_slot;
					if (evts[i].type) {
						temp_slot.tipo = "d";
						tipo_cap_slot = 0;
					}
					else {
						temp_slot.tipo = "a";
						tipo_cap_slot = 1;
					}
					temp_slot.key_slot = evts[i].local + to_string(evts[i].h_slot) + temp_slot.tipo;
					int h_busca = evts[i].h_slot % 24;
					string aep_temp = evts[i].local;
					int aep = distance(aerps.begin(), find_if(aerps.begin(), aerps.end(), [aep_temp](const airport_struct& a) { return a.nome == aep_temp; }));
					//temp_slot.rest = aerps[aep].slot_lim[h_busca][tipo_cap_slot];
					
					if (tipo_cap_slot == 0)
						temp_slot.rest = airpcap[aep].depcap[evts[i].h_slot];
					else
						temp_slot.rest = airpcap[aep].arrvcap[evts[i].h_slot];
					
					int p = distance(lista_cfg2.begin(), it);
					string busc("");
					GRBLinExpr expr = 0;
					int gbpos = vecgbxfc2(arctoflpos(evts[i].key_flight), arctoopc(evts[i].key_flight), 1, 0);
					expr += xfc[gbpos];
					temp_slot.name = "Slot_" + temp_slot.key_slot;
					temp_slot.expr = expr;
					temp_slot.constraint = "Slot_" + temp_slot.key_slot + " : " + busc;
					string key_temp = temp_slot.key_slot;
					auto it = find_if(slotconsts.begin(), slotconsts.end(), [key_temp](const slot_struct& a) { return a.key_slot == key_temp; });
					if (it == slotconsts.end())
						slotconsts.push_back(temp_slot);
					else {
						int pos = distance(slotconsts.begin(), it);
						slotconsts[pos].constraint += busc;
						slotconsts[pos].name = temp_slot.name;
						slotconsts[pos].expr += temp_slot.expr;
					}
				}
			}
		}
		cout << "Slot Capacity Constraint" << endl;
	
		int nhacft = 0;
		if (alt_acfts) {
			for (int i = 0; i < altacft.size(); i++) {
				int it_cfg = altacft[i].cfg;
				auto it = find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg);
				if (it != lista_cfg2.end()) {
					GRBLinExpr expr = 0;
					//int cnfmat = distance(lista_cfg2.begin(), it);
					string texto = "";
					string constraintName = "Acft_unavailabe_" + to_string(it_cfg) + "_" + to_string(i);
					//for (int k = 0; k < matrics[cnfmat].size(); k++) {
						for (int j = 0; j < altacft[i].flids.size(); j++) {
							int gbpos = vecgbxfc2(flidtoflpos(altacft[i].flids[j]), 0, 0, 0);
							expr += xfc[gbpos];
						}
					//}
					model.addConstr(expr == 1, constraintName);
					nhacft += (altacft[i].t2 - altacft[i].t1) / 60;
				}
			}
		}
		cout << "Aircraft Unavailable Constraint" << endl;
		if (alt_flights) {
			for (int i = 0; i < altflts.size(); i++) {
				int it_cfg = conf_flid(altflts[i].flid);
				auto it = find(lista_cfg2.begin(), lista_cfg2.end(), it_cfg);
				if (it != lista_cfg2.end()) {
					GRBLinExpr expr = 0;
					int res_gb;
					int cnfmat = distance(lista_cfg2.begin(), it);
					string flt_temp = altflts[i].flid;
					string texto = "";
					int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
					string constraintName = "Altflight_" + to_string(flts[flt_pos].cfg) + "_" + altflts[i].flid;
					if (altflts[i].atr > 0) {
							for (int j = 0; j < altflts[i].atr_int; j++) {
								int gbpos = vecgbxfc2(flidtoflpos(altflts[i].flid), j, 1, 0);
								expr += xfc[gbpos];
							}
						res_gb = 0;
					}
					else {
						texto += "yf_" + to_string(flts[flt_pos].cfg) + "_" + altflts[i].flid;
						texto += " = 1";
						string buscado = altflts[i].flid;
						int gbpos = vecgbyf(buscado);
						expr += yf[gbpos];
						res_gb = 1;
					}
					model.addConstr(expr == res_gb, constraintName);
				}
			}
		}

		int ncapred = 0; int n15 = 0; int n30 = 0; int n60 = 0; int nm60 = 0; int nswap = 0; int ncancpos = 0;
		cout << "Flight perturbations Constraint" << endl;
				
		if (alt_arps)
			for (int i = 0; i < altslots.size(); i++) {
				for (int horaslot = altslots[i].h1; horaslot < altslots[i].h2; horaslot++) {
					// departure
					string keyslot = altslots[i].airp + to_string(horaslot) + "d";
					auto its = find_if(slotconsts.begin(), slotconsts.end(), [keyslot](const slot_struct& a) { return a.key_slot == keyslot; });
					if (its != slotconsts.end()) {
						int slot_pos = distance(slotconsts.begin(), its);
						//slotconsts[slot_pos].rest = altslots[i].dep;
						slotconsts[slot_pos].constraint = "Alt_" + slotconsts[slot_pos].constraint;
						slotconsts[slot_pos].name = "Alt_" + slotconsts[slot_pos].name;
					}
					// arrival
					keyslot = altslots[i].airp + to_string(horaslot) + "a";
					its = find_if(slotconsts.begin(), slotconsts.end(), [keyslot](const slot_struct& a) { return a.key_slot == keyslot; });
					if (its != slotconsts.end()) {
						int slot_pos = distance(slotconsts.begin(), its);
						//slotconsts[slot_pos].rest = altslots[i].arrv;
						slotconsts[slot_pos].constraint = "Alt_" + slotconsts[slot_pos].constraint;
						slotconsts[slot_pos].name = "Alt_" + slotconsts[slot_pos].name;
					}
					ncapred++;
				}
				
			}

		for (int i = 0; i < slotconsts.size(); i++)
			model.addConstr(slotconsts[i].expr <= slotconsts[i].rest, slotconsts[i].name);
		
		cout << "Airport Perturbations Constraint" << endl;
		pos_rest = true; //Ativa restricao de posicionamento
		//  positioned ACFT airport  restriction
		if (pos_rest) {
			int dif_pos = 0;
			for (int p = 0; p < lista_cfg2.size(); p++) {
				int it_cfg = lista_cfg2[p];
				for (int i = 0; i < nos[it_cfg].size(); i++) {
					if (nos[it_cfg][i].size() > 0) {
						GRBLinExpr expr = 0;
						string constraintName = "PosAcfts_" + to_string(it_cfg) + "_" + aerps[i].nome;
						int timeend = recovery_time.end + cfgs[it_cfg].tat;
						int posrcv = distance(nos[it_cfg][i].begin(), find_if(nos[it_cfg][i].begin(), nos[it_cfg][i].end(), [timeend](const activity_struct& a) { return a.timee == timeend; }));
						int realpos = nos[it_cfg][i][posrcv].outgrdarc_int[0];
						int orgpos = 0;
						if (aerps[i].acft_pos.size() > 0)
							orgpos = aerps[i].acft_pos[it_cfg];
						int gbpos = vecgbzga(realpos, 0);
						expr += zga[gbpos];
						model.addConstr(expr == orgpos, constraintName);
					}
				}
			}
			cout << "Position ACFT airport Perturbations Constraint" << endl;
		}


		
		cout << "All Coinstraints" << endl;
		model.update();

		cout << "Objective Function" << endl << endl;
		model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
		model.update();
		model.write(nome_lp);
		model.set(GRB_DoubleParam_TimeLimit, max_time);
		model.getEnv().set(GRB_DoubleParam_MIPGap, min_gap);
		model.optimize();
		string nome_result_assign = "./Results/" + nome_instancia + "/" + nome_instancia + "_" + to_string(noptions) + ".sol";
		model.write(nome_result_assign);
		res_file_gnr << "NOME: " << nome_instancia << endl << "NACFTS: " << acfts.size() - 1 << endl;
		

		
		if (model.get(GRB_IntAttr_SolCount) == 0) {
			feasibility = 0;
			res_file_gnr << "ASSIGNMENT" << endl << "FEASIBLE: " << feasibility << endl;
		}
		else {
			feasibility = 1;
			gap1 = abs(model.get(GRB_DoubleAttr_ObjBound) - model.get(GRB_DoubleAttr_ObjVal)) / abs(model.get(GRB_DoubleAttr_ObjVal));
			result = model.get(GRB_DoubleAttr_ObjVal);
			timerun = model.get(GRB_DoubleAttr_Runtime);
			optimstatus = model.get(GRB_IntAttr_Status);

			//result = model.get(GRB);
			cout << "Resultado: " << result << endl << "GAP: " << gap1 << endl;
			nflights = 0;
			for (int i = 0; i < flts.size(); i++)
				if (find(lista_cfg2.begin(), lista_cfg2.end(), flts[i].cfg) != lista_cfg2.end()) {
					if (!flts[i].mant) {
						if (yf[nflights].get(GRB_DoubleAttr_X) > 0.0001) {
							flts[i].result = -1;
							flts[i].cost = cancel_cost;
							ncancpos++;
						}
						if (flts[i].atr > 0)
							init_delay += flts[i].atr;
						if (flts[i].atr < 0)
							ncanc++;

						nflights++;
					}
				}

			//cout << "yf contabilizada" << endl;

			int final_delay = 0;
			h = 0;
			for (int p = 0; p < lista_cfg2.size(); p++) {
				int it_cfg = lista_cfg2[p];
				for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
					string flt_temp = cfgs[it_cfg].fltids[i];
					int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
					// flts[flt_pos].inic_xfc = h;
					for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
							if (xfc[h].get(GRB_DoubleAttr_X) > 0.0001) {
								flts[flt_pos].result = j;
								flts[flt_pos].cost = minute_cost * min_opt * j;
								if (flts[flt_pos].result == 1) n15++;
								if ((flts[flt_pos].result == 2) || (flts[flt_pos].result == 3)) n30++;
								if (flts[flt_pos].result == 4) n60++;
								if (flts[flt_pos].result > 4) nm60++;
								final_delay += min_opt * j;
							}
							h++;
					}
				}
			}
			
			cout << "xfc contabilizada" << endl;
			h = 0;
			for (int p = 0; p < namesga.size(); p++) {
				auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
				if (it != lista_cfg2.end()) {
					if (zga[h].get(GRB_DoubleAttr_X) > 0.0001) {
							results_namesga[p] = zga[h].get(GRB_DoubleAttr_X);
							struct_gas tempgas;
							tempgas.name = namesga[p];
							tempgas.result = 1;
							tempgas.pos = p;
							gas[conf_namesga[p]].push_back(tempgas);
					}
					else
						results_namesga[p] = 0;
					
					h++;
				}
			}
			cout << "zga contabilizada" << endl;
			org_results_namesga = results_namesga;
		
			cout << "ASSIGNED Positioned Aircrafts" << endl << " cfg    airp    ORG_POSI   FINAL_POSI" << endl;
			// Calculate non positioned airport
	     	int dif_pos = 0;
			for (int p = 0; p < lista_cfg2.size(); p++) {
				int it_cfg = lista_cfg2[p];
				for (int i = 0; i < nos[it_cfg].size(); i++) {
					if (nos[it_cfg][i].size() > 0) {
						int timeend = recovery_time.end + cfgs[it_cfg].tat;
						int posrcv = distance(nos[it_cfg][i].begin(), find_if(nos[it_cfg][i].begin(), nos[it_cfg][i].end(), [timeend](const activity_struct& a) { return a.timee == timeend; }));
						int realpos = nos[it_cfg][i][posrcv].outgrdarc_int[0];
						int orgpos = 0;
						if (aerps[i].acft_pos.size() > 0)
							orgpos = aerps[i].acft_pos[it_cfg];
						dif_pos += abs(results_namesga[realpos] - orgpos);
						cout << it_cfg << " " << nos[it_cfg][i][0].local << " " << orgpos << " " << results_namesga[realpos] << endl;
					}
				}
			}        
			cout << endl;
			
			res_file_gnr << "NFLIGHT: " << nflights << endl << "INIT_FLIGHT_DELAY: " << init_delay << endl;
			res_file_gnr << "NCANC: " << ncanc << endl << "N_HOURS_AIRPORT_CAPACITY_REDUCTION: " << ncapred << endl << "N_HOURS_ACFT_BROKEN_PERIOD: " << nhacft << endl;
			res_file_gnr << "ASSIGNMENT" << endl << "FEASIBLE: " << feasibility << endl << "RESULT: " << result << endl << "GAP: " << gap1 << endl;
			res_file_gnr << "Time: " << timerun << endl << "Status: " << optimstatus << endl << "NCANC_POS: " << ncancpos << endl;
			res_file_gnr << "Final_Delay: " << final_delay << endl << "NATZ15: " << n15 << endl;
			res_file_gnr << "NATZ30: " << n30 << endl << "NATZ60: " << n60 << endl;
			res_file_gnr << "NATZM60: " << nm60 << endl << "NON POSITIONED AIRCFT: " << dif_pos << endl ;

			tempassign.NFLIGHT = nflights;
			tempassign.NATZ15 = n15;
			tempassign.NATZ30 = n30;
			tempassign.NATZ60 = n60;
			tempassign.NATZM60 = nm60;
			tempassign.dif_pos = dif_pos;
			tempassign.NCANC_POS = ncancpos;
			tempassign.final_delay = final_delay;
			tempassign.NCANC = ncanc;
			tempassign.N_HOURS_ACFT_BROKEN_PERIOD = nhacft;
			tempassign.N_HOURS_AIRPORT_CAPACITY_REDUCTION = ncapred;
			tempassign.dif_pos = dif_pos;
		}
		tempassign.FEASIBLE = feasibility;
		tempassign.RESULT_ASSIGN = result;
		tempassign.GAP = gap1;
		tempassign.Time = timerun;
		tempassign.Status = optimstatus;
		tempassign.countsol = model.get(GRB_IntAttr_SolCount);
		tempassign.INIT_FLIGHT_DELAY = init_delay;


		cout << "Objective function added" << endl;
		time_t now1 = time(0);
		model.reset();
	}
	catch (GRBException e)
	{
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...)
	{
		cout << "Exception during optimization" << endl;
	}

	//lp.close();
	res_file_gnr.close();
	return tempassign;
}

// ROUTING ALL CONFIGURATIONS TOGETHER GUROBI
void create_lp_routing_gurobi2() {

	

	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	ofstream res_file_gnr((char *)arqresgeral.c_str(), std::fstream::app);

	vector <int> lista_cfg2;
	for (int i = 0; i<cfgs.size(); i++)
		if (cfgs[i].model != "TranspCom")
			lista_cfg2.push_back(i);

	inic_gbga.clear();

	try {



		GRBEnv *env = NULL; // creates the GUROBI environment
		env = new GRBEnv();
		GRBModel model2 = GRBModel(*env);    // creates an empty model


		GRBVar *yf = NULL;
		yf = new GRBVar[flts.size()]; //cancel decision variable


		int max_nflcfg = -1;
		for (int i = 0; i < lista_cfg2.size(); i++) {
			int id_cfg = lista_cfg2[i];
			int nflcfg = count_if(acfts.begin(), acfts.end(), [id_cfg](const aircraft_struct& a) { return a.cfg == id_cfg; });
			if (nflcfg > max_nflcfg)
				max_nflcfg = nflcfg;
		}

		GRBVar *xfc = NULL;
		xfc = new GRBVar[flts.size()*noptions*max_nflcfg]; //flight decision variable

		GRBVar *zga = NULL;
		zga = new GRBVar[namesga.size()*max_nflcfg]; //ground arc decision variable       

		model2.update();


		for (int i = 0; i < lista_cfg2.size(); i++) {
			int id_cfg = lista_cfg2[i];
			vector < int > temp_matrics;
			for (int j = 0; j < acfts.size(); j++)
				if (acfts[j].cfg == id_cfg)
					temp_matrics.push_back(j);
			matrics.push_back(temp_matrics);
		}
		cout << "LP Routing" << endl << endl;
		GRBLinExpr expr_obj = 0;
		string string_bin = "";
	


		int h = 0;
		for (int p = 0; p < lista_cfg2.size(); p++) {
			int it_cfg = lista_cfg2[p];
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
				flts[flt_pos].inic_xfc = h;
				string acf = flts[flt_pos].org_acft;
				int acftpos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [acf](const aircraft_struct& a) { return a.id == acf; }));
				for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
					for (int k = 0; k < matrics[p].size(); k++) {
						string varName = "xfc_" + to_string(flts[flt_pos].cfg) + "_" + flts[flt_pos].kfs[j] + "_" + to_string(matrics[p][k]);
						int changecost = 0;
						if (matrics[p][k] != acftpos)
							changecost = 1;
						if ((find(lista_cfg.begin(), lista_cfg.end(), it_cfg) != lista_cfg.end())&&(flts[flt_pos].result==j))
							xfc[h] = model2.addVar(0.0, 1.0, changecost, GRB_BINARY, varName);
						else
							xfc[h] = model2.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
						h++;
					}
				}
			}
		}
		cout << "xfc variables added" << endl;

		h = 0;
		for (int p = 0; p < namesga.size(); p++) {
			auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
			if (it != lista_cfg2.end()) {
				int cnfmat = distance(lista_cfg2.begin(), it);
				inic_gbga.push_back(h);
				for (int k = 0; k < matrics[cnfmat].size(); k++) {
					string varName = namesga[p] + "_" + to_string(matrics[cnfmat][k]);
					zga[h] = model2.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
					h++;
				}
			}
		}
		cout << "zga variables added" << endl;

		model2.update();

		//Balance Flow Constraint  
		vector < string >  string_rest;
		string text_rest("");
		for (int p = 0; p < lista_cfg.size(); p++) {
			int it_cfg = lista_cfg[p];
			for (int i = 0; i < nos[it_cfg].size(); i++)
				for (int k = 0; k < nos[it_cfg][i].size(); k++) {
					if (nos[it_cfg][i][k].key.substr(0, 3) != "OUT") {
						for (int mat = 0; mat < matrics[p].size(); mat++) {

							bool achou = false;
							GRBLinExpr expr = 0;
							string constraintName = "BF_" + to_string(it_cfg) + "_" + nos[it_cfg][i][k].key + "_" + to_string(matrics[p][mat]);

							for (int j = 0; j < nos[it_cfg][i][k].infltarc.size(); j++) {
								int flt_temp = nos[it_cfg][i][k].infltarc_int[j].flidpos;
								int opc = nos[it_cfg][i][k].infltarc_int[j].opc;
								if (flts[flt_temp].result == opc) {
									int opc = nos[it_cfg][i][k].infltarc_int[j].opc;
									int gbpos = vecgbxfc2(flt_temp, opc, matrics[p].size(), mat);
									expr += xfc[gbpos];
									achou = true;
								}
							}
							for (int j = 0; j < nos[it_cfg][i][k].ingrdarc.size(); j++) {
								int flt_pos = nos[it_cfg][i][k].ingrdarc_int[j];
								if (results_namesga[flt_pos] > 0) {
									int gbpos = vecgbzga(flt_pos, mat);
									expr += zga[gbpos];
									achou = true;
								}
							}
							for (int j = 0; j < nos[it_cfg][i][k].outfltarc.size(); j++) {
								int flt_temp = nos[it_cfg][i][k].outfltarc_int[j].flidpos;
								int opc = nos[it_cfg][i][k].outfltarc_int[j].opc;
								if (flts[flt_temp].result == opc) {
									int opc = nos[it_cfg][i][k].outfltarc_int[j].opc;
									int gbpos = vecgbxfc2(flt_temp, opc, matrics[p].size(), mat);
									expr -= xfc[gbpos];
									achou = true;
								}
							}

							for (int j = 0; j < nos[it_cfg][i][k].outgrdarc.size(); j++) {
								int flt_pos = nos[it_cfg][i][k].outgrdarc_int[j];
								if (results_namesga[flt_pos] > 0) {
									int gbpos = vecgbzga(flt_pos, mat);
									expr -= zga[gbpos];
									achou = true;
								}
							}
							if (achou) {
								int aern = matrics[p][mat];
								string res_str;
								int res_int;
								if ((acfts[aern].airp == nos[it_cfg][i][k].local) && (k == 0)) {
									res_str = " = - 1";
									res_int = -1;
								}
								else {
									res_str = " = 0";
									res_int = 0;
								}
								model2.addConstr(expr == res_int, constraintName);
							}
						}
					}
				}
		}
		cout << "Balance Flow Constraint Added" << endl;


		//Cancel or Delay Constraints
		for (int p = 0; p < lista_cfg.size(); p++) {
			int it_cfg = lista_cfg[p];
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				int flt_pos = flidtoflpos(flt_temp);
				if (flts[flt_pos].result >= 0) {
					string fl_temp = flt_temp + "_" + to_string(flts[flt_pos].result);
					if (!mant_flid(flt_temp)) {
						GRBLinExpr expr = 0;
						string constraintName = "CancelDelay_" + to_string(it_cfg) + "_" + flt_temp;
						for (int mat = 0; mat < matrics[p].size(); mat++) {
							int gbpos2 = vecgbxfc2(arctoflpos(fl_temp), arctoopc(fl_temp), matrics[p].size(), mat);
							expr += xfc[gbpos2];
						}
						model2.addConstr(expr == 1, constraintName);
					}
					else
						if (mant_flid(flt_temp)) {
							GRBLinExpr expr = 0;
							string tmpacft = flts[flt_pos].org_acft;
							int acft_pos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [tmpacft](const aircraft_struct& a) { return a.id == tmpacft; }));
							// maintenace couverture
							string constraintName = "Maintenance_Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
							for (int k = 0; k < matrics[p].size(); k++) {
								if (matrics[p][k] == acft_pos) {
									int gbpos2 = vecgbxfc2(flt_pos, 0, matrics[p].size(), k);
									expr += xfc[gbpos2];
									model2.addConstr(expr == 1, constraintName);
									break;
								}
							}
						}
				}
			}
		}
		cout << "Coverture - Cancel or Delay Constraints" << endl;


		//int ncapred = 0; int n15 = 0; int n30 = 0; int n60 = 0; int nm60 = 0; int nswap = 0; int ncancpos = 0;

		
		cout << "All Coinstraints" << endl;
		model2.update();

		cout << "Objective Function" << endl << endl;
		model2.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
		model2.update();
		model2.write(nome_lp_fifo + ".lp");
		model2.set(GRB_DoubleParam_TimeLimit, max_time);
		model2.optimize();
		int feasibility = 0;
		double gap1 = 0;
		int optimstatus;
		double result = 0;
		double result2 = 0;
		double timerun;
		int ncanc = 0;

		if (model2.get(GRB_IntAttr_SolCount) == 0) {
			//cout << "No solution found, optimization status = " << model.get(GRB_IntAttr_Status) << endl;
			feasibility = 0;
		}
		else {
			feasibility = 1;
			gap1 = model2.get(GRB_DoubleParam_MIPGap);
			result = model2.get(GRB_DoubleAttr_ObjVal);
			timerun = model2.get(GRB_DoubleAttr_Runtime);
			optimstatus = model2.get(GRB_IntAttr_Status);

			int h = 0;
			for (int p = 0; p < lista_cfg2.size(); p++) {
				int it_cfg = lista_cfg2[p];
				for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
					string flt_temp = cfgs[it_cfg].fltids[i];
					int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
					//flts[flt_pos].inic_xfc = h;
					string acf = flts[flt_pos].org_acft;
					int acftpos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [acf](const aircraft_struct& a) { return a.id == acf; }));
					for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
						for (int k = 0; k < matrics[p].size(); k++) {
							if (xfc[h].get(GRB_DoubleAttr_X) > 0.0001)
								flts[flt_pos].final_acft = acfts[matrics[p][k]].id;
							h++;
						}
					}
				}
			}

			h = 0;
			for (int p = 0; p < namesga.size(); p++) {
				auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
				if (it != lista_cfg2.end()) {
					int cnfmat = distance(lista_cfg2.begin(), it);
					for (int k = 0; k < matrics[cnfmat].size(); k++) {
						if (zga[h].get(GRB_DoubleAttr_X) > 0.0001)
							acfts_namesga[p] = matrics[cnfmat][k];
						h++;
					}
				}
			}

		}
	}
	catch (GRBException e)
	{
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...)
	{
		cout << "Exception during optimization" << endl;
	}

}

// ROUTING SEPARATED CONFIGURATIONS GUROBI
rodada_struct create_lp_routing_separada_gurobi2(rodada_struct temprod) {


	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	ofstream res_file_gnr((char *)arqresgeral.c_str(), std::fstream::app);

	vector <int> lista_cfg2;
	for (int i = 0; i<cfgs.size(); i++)
		if (cfgs[i].model != "TranspCom")
			lista_cfg2.push_back(i);

	inic_gbga.clear();
	double res_tot = 0;
	vector < int > feasvec, timevec, optmvec,mantvec;
	vector < string > confvec;
	vector < double > resultvec;
	int gnr_feasible = 1;
	int ncons = 0;
	int max_xfc = 0;
	try {

		cout << "LP Routing Assignment" << endl << endl;
		GRBEnv *env = NULL; // creates the GUROBI environment
		env = new GRBEnv();

		GRBModel model2 = GRBModel(*env);    // creates an empty model

		GRBVar *yf = NULL;
		yf = new GRBVar[flts.size()]; //cancel decision variable

		int max_nflcfg = -1;
		for (int i = 0; i < lista_cfg2.size(); i++) {
			int id_cfg = lista_cfg2[i];
			int nflcfg = count_if(acfts.begin(), acfts.end(), [id_cfg](const aircraft_struct& a) { return a.cfg == id_cfg; });
			if (nflcfg > max_nflcfg)
				max_nflcfg = nflcfg;
		}

		GRBVar *xfc = NULL;
		xfc = new GRBVar[flts.size()*noptions*max_nflcfg]; //flight decision variable

		GRBVar *zga = NULL;
		zga = new GRBVar[namesga.size()*max_nflcfg]; //ground arc decision variable       

		model2.update();


		for (int i = 0; i < lista_cfg2.size(); i++) {
			int id_cfg = lista_cfg2[i];
			vector < int > temp_matrics;
			for (int j = 0; j < acfts.size(); j++)
				if (acfts[j].cfg == id_cfg)
					temp_matrics.push_back(j);
			matrics.push_back(temp_matrics);
		}

		cout << "Matrics LISTA2" << endl;
		for (size_t i = 0; i < matrics.size(); i++) {
			cout << endl << "conf " << lista_cfg2[i] << ": ";
			for (size_t j = 0; j < matrics[i].size(); j++)
				cout << matrics[i][j] << " ";
		}
		cout << endl << endl;
		
		//GRBLinExpr expr_obj = 0;

		int h = 0;
		for (int p = 0; p < lista_cfg2.size(); p++) {
			int it_cfg = lista_cfg2[p];
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
				flts[flt_pos].inic_xfc = h;
				string acf = flts[flt_pos].org_acft;
				int acftpos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [acf](const aircraft_struct& a) { return a.id == acf; }));
				for (int j = 0; j < flts[flt_pos].kfs.size(); j++) {
					for (int k = 0; k < matrics[p].size(); k++) {
						string varName = "xfc_" + to_string(flts[flt_pos].cfg) + "_" + flts[flt_pos].kfs[j] + "_" + to_string(matrics[p][k]);
						int changecost = 0;
						if (matrics[p][k] != acftpos)
							changecost = 1;
						if (flts[flt_pos].result == j)
							xfc[h] = model2.addVar(0.0, 1.0, changecost, GRB_BINARY, varName);
						else
							xfc[h] = model2.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
						h++;
					}
				}
			}
		}
		cout << "xfc variables added" << endl;
		max_xfc = h;
		h = 0;
		for (int p = 0; p < namesga.size(); p++) {
			auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
			if (it != lista_cfg2.end()) {
				int cnfmat = distance(lista_cfg2.begin(), it);
				inic_gbga.push_back(h);
				for (int k = 0; k < matrics[cnfmat].size(); k++) {
					string varName = namesga[p] + "_" + to_string(matrics[cnfmat][k]);
					zga[h] = model2.addVar(0.0, 1.0, 0.0, GRB_BINARY, varName);
					h++;
				}
			}
		}
		cout << "zga variables added" << endl;

		model2.update();


		vector <GRBConstr> constr;
		res_file_gnr << "ROUTING";
		for (int pe = 0; pe < lista_cfg.size(); pe++) {
		
			int id_cfd_sep = lista_cfg[pe];
			ncons = 0;
			string tmpcnf = cfgs[id_cfd_sep].model + to_string(cfgs[id_cfd_sep].ass);
			confvec.push_back(tmpcnf);
			temprod.confvec.push_back(tmpcnf);
			cout << "LP Routing " << cfgs[id_cfd_sep].model + to_string(cfgs[id_cfd_sep].ass) << endl;
			

			//Balance Flow Constraint  
			vector < string >  string_rest;

			int it_cfg = id_cfd_sep;
			int p = pe;

			for (int i = 0; i < nos[it_cfg].size(); i++) {
				for (int k = 0; k < nos[it_cfg][i].size(); k++) {
					if (nos[it_cfg][i][k].key.substr(0, 3) != "OUT") {
						for (int mat = 0; mat < matrics[p].size(); mat++) {

							bool achou = false;
							GRBLinExpr expr = 0;
							string constraintName = "BF_" + to_string(it_cfg) + "_" + nos[it_cfg][i][k].key + "_" + to_string(matrics[p][mat]);

							for (int j = 0; j < nos[it_cfg][i][k].infltarc.size(); j++) {
								int flt_temp = nos[it_cfg][i][k].infltarc_int[j].flidpos;
								int opc = nos[it_cfg][i][k].infltarc_int[j].opc;
								if (flts[flt_temp].result == opc) {
									int opc = nos[it_cfg][i][k].infltarc_int[j].opc;
									int gbpos = vecgbxfc2(flt_temp, opc, matrics[p].size(), mat);
									expr += xfc[gbpos];
									//cout << expr << endl;
									achou = true;
								}
							}
							for (int j = 0; j < nos[it_cfg][i][k].ingrdarc.size(); j++) {
								int flt_pos = nos[it_cfg][i][k].ingrdarc_int[j];
								if (results_namesga[flt_pos] > 0) {
									int gbpos = vecgbzga(flt_pos, mat);
									expr += zga[gbpos];
									//cout << expr << endl;
									achou = true;
								}
							}
							for (int j = 0; j < nos[it_cfg][i][k].outfltarc.size(); j++) {
								int flt_temp = nos[it_cfg][i][k].outfltarc_int[j].flidpos;
								int opc = nos[it_cfg][i][k].outfltarc_int[j].opc;
								if (flts[flt_temp].result == opc) {
									int opc = nos[it_cfg][i][k].outfltarc_int[j].opc;
									int gbpos = vecgbxfc2(flt_temp, opc, matrics[p].size(), mat);
									expr -= xfc[gbpos];
									achou = true;
								}
							}
							for (int j = 0; j < nos[it_cfg][i][k].outgrdarc.size(); j++) {
								int flt_pos = nos[it_cfg][i][k].outgrdarc_int[j];
								if (results_namesga[flt_pos] > 0) {
									int gbpos = vecgbzga(flt_pos, mat);
									expr -= zga[gbpos];
									achou = true;
								}
							}
							if (achou) {
								int aern = matrics[p][mat];
								string res_str;
								int res_int;
								if ((acfts[aern].airp == nos[it_cfg][i][k].local) && (k == 0)) {
									res_str = " = - 1";
									res_int = -1;
								}
								else {
									res_str = " = 0";
									res_int = 0;
								}
								GRBConstr tempcons = model2.addConstr(expr == res_int, constraintName);
								ncons++;
								constr.push_back(tempcons);
							}
						}
					}
				}
			}
			cout << "Balance Flow Constraint Added" << endl;


			//Cancel or Delay Constraints
			int ncanccfg = 0, n15cfg = 0, n30cfg = 0, n60cfg = 0, nm60cfg = 0;
			int nmant = 0;
			int ncons = 0;
			it_cfg = id_cfd_sep;
			p = pe;
			for (int i = 0; i < cfgs[it_cfg].fltids.size(); i++) {
				string flt_temp = cfgs[it_cfg].fltids[i];
				int flt_pos = flidtoflpos(flt_temp);
				if (flts[flt_pos].result >= 0) {
					string fl_temp = flt_temp + "_" + to_string(flts[flt_pos].result);
					if (!mant_flid(flt_temp)) {
						GRBLinExpr expr = 0;
						string constraintName = "CancelDelay_" + to_string(it_cfg) + "_" + flt_temp;
						for (int mat = 0; mat < matrics[p].size(); mat++) {
							int gbpos2 = vecgbxfc2(arctoflpos(fl_temp), arctoopc(fl_temp), matrics[p].size(), mat);
							expr += xfc[gbpos2];
						}
						GRBConstr tempcons = model2.addConstr(expr == 1, constraintName);
						ncons++;
						constr.push_back(tempcons);
						if (flts[flt_pos].result == 1) n15cfg++;
						if (flts[flt_pos].result == 2) n30cfg++;
						if ((flts[flt_pos].result == 4) || (flts[flt_pos].result == 3)) n60cfg++;
						if (flts[flt_pos].result > 4) nm60cfg++;
					}
					else
						if (mant_flid(flt_temp)) {
							nmant++;
							GRBLinExpr expr = 0;
							string tmpacft = flts[flt_pos].org_acft;
							int acft_pos = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [tmpacft](const aircraft_struct& a) { return a.id == tmpacft; }));
							// maintenace couverture
							string constraintName = "Maintenance_Flight_Couverture_" + to_string(it_cfg) + "_" + flt_temp;
							for (int k = 0; k < matrics[p].size(); k++) {
								if (matrics[p][k] == acft_pos) {
									int gbpos2 = vecgbxfc2(flt_pos, 0, matrics[p].size(), k);
									expr += xfc[gbpos2];
									GRBConstr tempcons = model2.addConstr(expr == 1, constraintName);
									constr.push_back(tempcons);
									ncons++;
									break;
								}
							}
						}
				}
				else
					ncanccfg++;
			}
			cout << "Coverture - Cancel or Delay Constraints" << endl;

			//int ncapred = 0; int n15 = 0; int n30 = 0; int n60 = 0; int nm60 = 0; int nswap = 0; int ncancpos = 0;

			cout << "All Coinstraints" << endl;
			model2.update();
			cout << "Objective Function" << endl << endl;
			model2.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);
			model2.update();
			string nome_lp_fifo2 = nome_lp_fifo + "_" + cfgs[id_cfd_sep].model + to_string(cfgs[id_cfd_sep].ass) + ".lp";
			model2.write(nome_lp_fifo2);
			model2.set(GRB_DoubleParam_TimeLimit, max_time);
			
			model2.optimize();
			string nome_lp_fifo2_sol = nome_lp_fifo + "_" + cfgs[id_cfd_sep].model + to_string(cfgs[id_cfd_sep].ass) + ".sol";
			//model2.write(nome_lp_fifo2_sol);
			int feasibility = 0;
			double gap1 = 0;
			int optimstatus = 0;
			double result = 0;
			double result2 = 0;
			double timerun;
			int ncanc = 0;
			

			
			if (model2.get(GRB_IntAttr_SolCount) == 0) {
				cout << "No solution found, optimization status = " << model2.get(GRB_IntAttr_Status) << endl;
				feasibility = 0;
			}
			else {
				feasibility = 1;
				gap1 = model2.get(GRB_DoubleParam_MIPGap);
				optimstatus = model2.get(GRB_IntAttr_Status);
				timerun = model2.get(GRB_DoubleAttr_Runtime);
				result = model2.get(GRB_DoubleAttr_ObjVal);


				for (int h=0; h < max_xfc; h++)
					if (xfc[h].get(GRB_DoubleAttr_X) > 0.0001) {
						string namevar = xfc[h].get(GRB_StringAttr_VarName);
						vector<string> sep = split(namevar, '_');
						string flid = sep[2];
						int cnfv = stoi(sep[1]);
						int opc = stoi(sep[3]);
						int mat = stoi(sep[4]);
						int fltpos = flidtoflpos(flid);
						if ((cnfv == id_cfd_sep)&&(flts[fltpos].result==opc)) {
							flts[fltpos].final_acft = acfts[mat].id;
						}
					}


				h = 0;
				for (int p = 0; p < namesga.size(); p++) {
					auto it = find(lista_cfg2.begin(), lista_cfg2.end(), conf_namesga[p]);
					if (it != lista_cfg2.end()) {
						int cnfmat = distance(lista_cfg2.begin(), it);
						for (int k = 0; k < matrics[cnfmat].size(); k++) {
							if ((zga[h].get(GRB_DoubleAttr_X) > 0.0001) && (conf_namesga[p] = id_cfd_sep))
								acfts_namesga[p] = matrics[cnfmat][k];
							h++;
						}
					}
				}
			}

			//store fifo results
			timevec.push_back(timerun);
			optmvec.push_back(optimstatus);
			feasvec.push_back(feasibility);
			resultvec.push_back(result);
			mantvec.push_back(nmant);
			temprod.timevec.push_back(timerun);
			temprod.optmvec.push_back(optimstatus);
			temprod.feasvec.push_back(feasibility);
			temprod.mantvec.push_back(nmant);
			temprod.resultvec.push_back(result);
			temprod.nfltsvec.push_back(ncons);
			temprod.n15vec.push_back(n15cfg);
			temprod.n30vec.push_back(n30cfg);
			temprod.n60vec.push_back(n60cfg);
			temprod.nm60vec.push_back(nm60cfg);
			temprod.ncancvec.push_back(ncanccfg);
	
			
			int buscado = id_cfd_sep;
			int nacf = count_if(acfts.begin(), acfts.end(), [buscado](const aircraft_struct& a) {  return a.cfg == buscado; });
			temprod.nacftsvec.push_back(nacf);

			//clean model to create a new one
			for (int p = 0; p < constr.size(); p++)
				model2.remove(constr[p]);
			model2.discardConcurrentEnvs();
			model2.reset();
			constr.clear();
			nmant = 0;
		}
	}
	catch (GRBException e)
	{
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...)
	{
		cout << "Exception during optimization" << endl;
	}

	res_tot = 0;
	for (int p = 0; p < flts.size(); p++)
		if (find(lista_cfg.begin(), lista_cfg.end(), flts[p].cfg) != lista_cfg.end())
			if (flts[p].result < 0) {
				res_tot += cancel_cost;
				flts[p].cost = cancel_cost;
			}
			else {
				res_tot += minute_cost * min_opt * flts[p].result;
				flts[p].cost += minute_cost * min_opt * flts[p].result;
				if (flts[p].final_acft != flts[p].org_acft) {
					res_tot += 1;
					flts[p].cost += 1;
				}
			}

	res_file_gnr << endl << "CONFIGS:     ";
	for (int k = 0; k < confvec.size(); k++) res_file_gnr << cplstr(confvec[k],11) << " ";
	res_file_gnr << endl << "FEASIBLE:    ";
	for (int k = 0; k < feasvec.size(); k++) {
		res_file_gnr << cplstr(to_string(feasvec[k]), 11) << " ";
		gnr_feasible = gnr_feasible * feasvec[k];
	}
	temprod.gnr_feas = gnr_feasible;
	temprod.TOTAL_RESULT = res_tot;
	temprod.NSWAPS = res_tot - temprod.RESULT_ASSIGN;
	res_file_gnr << endl << "RESULT:      ";
	for (int k = 0; k < resultvec.size(); k++) {
		res_file_gnr << cplstr(to_string(int(resultvec[k])), 11) << " ";
	}
	res_file_gnr << endl << "TIME:        ";
	for (int k = 0; k < timevec.size(); k++) res_file_gnr << cplstr(to_string(timevec[k]),11) << " ";
	res_file_gnr << endl << "STATUS:      ";
	for (int k = 0; k < optmvec.size(); k++) res_file_gnr << cplstr(to_string(optmvec[k]), 11) << " ";
	res_file_gnr << endl << "MANT NUMBER: ";
	for (int k = 0; k < mantvec.size(); k++) res_file_gnr << cplstr(to_string(mantvec[k]), 11) << " ";
	res_file_gnr << endl << "FLHT NUMBER: ";
	for (int k = 0; k < temprod.nfltsvec.size(); k++) res_file_gnr << cplstr(to_string(temprod.nfltsvec[k]), 11) << " ";
	res_file_gnr << endl << "ACFT NUMBER: ";
	for (int k = 0; k < temprod.nacftsvec.size(); k++) res_file_gnr << cplstr(to_string(temprod.nacftsvec[k]), 11) << " ";
	res_file_gnr << endl << "N15 NUMBER: ";
	for (int k = 0; k < temprod.n15vec.size(); k++) res_file_gnr << cplstr(to_string(temprod.n15vec[k]), 11) << " ";
	res_file_gnr << endl << "N30 NUMBER: ";
	for (int k = 0; k < temprod.n30vec.size(); k++) res_file_gnr << cplstr(to_string(temprod.n30vec[k]), 11) << " ";
	res_file_gnr << endl << "N60 NUMBER: ";
	for (int k = 0; k < temprod.n60vec.size(); k++) res_file_gnr << cplstr(to_string(temprod.n60vec[k]), 11) << " ";
	res_file_gnr << endl << "Nm60NUMBER: ";
	for (int k = 0; k < temprod.nm60vec.size(); k++) res_file_gnr << cplstr(to_string(temprod.nm60vec[k]), 11) << " ";
	res_file_gnr << endl << "NCNLNUMBER: ";
	for (int k = 0; k < temprod.ncancvec.size(); k++) res_file_gnr << cplstr(to_string(temprod.ncancvec[k]), 11) << " ";

	res_file_gnr << endl << "NSWAPS:      " << res_tot - temprod.RESULT_ASSIGN;
	res_file_gnr << endl << "General_Feasibility: " << gnr_feasible;
	res_file_gnr << endl << "Resultado_GERAL: " << res_tot << endl;
	res_file_gnr.close();
	return temprod;
}
