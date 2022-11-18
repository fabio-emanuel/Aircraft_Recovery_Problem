#ifndef __ESTRUTURAS__H__
#define __ESTRUTURAS__H__

#include "variaveis.h"
#include "extern.cpp"

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;
	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	return internal;
}

int count_diferents(vector<string> a) {
	vector<string> aux;
	for (int i = 0; i<a.size(); i++) {
		string temp = a[i];
		if (find(aux.begin(), aux.end(), temp) == aux.end())
			aux.push_back(temp);
	}
	return aux.size();
}

int hora_int(string tempo) {
	int oi = 0;
	if (tempo.size()>5) {
		oi = stoi(tempo.substr(6, 1));
		tempo = tempo.substr(0, 5);
	}
	int hour = stoi(tempo.substr(0, 2));
	int min = stoi(tempo.substr(3, 2));
	return oi * 1440 + hour * 60 + min;
}

string complstr(int h) {
	string st = to_string(h);
	if (st.length()<2)
		return "0" + st;
	else
		return st;
}

string int_hora(int tempo) {
	int oii = tempo / 1440;
	string oi = "";
	if (oii>0) oi = "+" + to_string(oii);
	int tempo_dia = tempo % 1440;
	int hora = tempo_dia / 60;
	int min = tempo_dia % 60;
	return complstr(hora) + ":" + complstr(min) + oi;
}

int fltps(string flid) {
	return distance(flts.begin(), find_if(flts.begin(), flts.end(), [flid](const flight_struct& a) { return a.flid == flid; }));
}

bool aerp_cfg(string aerp, int cfg) {
	bool resp = false;
	for (int j = 0; j<cfgs[cfg].fltids.size(); j++) {
		int p1 = fltps(cfgs[cfg].fltids[j]);
		if ((flts[p1].org == aerp) || (flts[p1].dest == aerp))
		{
			resp = true;
			break;
		}
	}
	return resp;
}

int flidtofln(string flt_temp) {
	return stoi(flt_temp.substr(flt_temp.find("d") + 1, flt_temp.npos));
}

int arctofln(string flt_temp) {
	string sttemp = flt_temp.substr(0, flt_temp.find("_"));
	return flidtofln(sttemp);
}

int arctoflpos(string flt_temp) {
	string sttemp = flt_temp.substr(0, flt_temp.find("_"));
	return distance(flts.begin(), find_if(flts.begin(), flts.end(), [sttemp](const flight_struct& a) { return a.flid == sttemp; }));
}

int arctoopc(string buscado) {
	int opc = stoi(buscado.substr(buscado.find_first_of("_") + 1, buscado.find_first_of("_", buscado.find_first_of("_") + 1)));
	return opc;
}

int flidtoflpos(string flt_temp) {
	return distance(flts.begin(), find_if(flts.begin(), flts.end(), [flt_temp](const flight_struct& a) { return a.flid == flt_temp; }));
}

bool mant_arc(string arc) {
	if (arctofln(arc)>max_fln)
		return true;
	else
		return false;
}

bool mant_flid(string flid) {
	if (flidtofln(flid)>max_fln)
		return true;
	else
		return false;
}

int conf_flid(string flid) {
	return flts[fltps(flid)].cfg;
}

bool flid_alt_acft(string flid) {
	if (flidtofln(flid)>max_fln_mnt)
		return true;
	else
		return false;
}

bool arc_result(string flt_temp) {
	int flpos = arctoflpos(flt_temp);
	int res_flts = flts[flpos].result;
	int res_arc = stoi(flt_temp.substr(flt_temp.find("_") + 1, flt_temp.npos));
	if (res_flts == res_arc)
		return true;
	else
		return false;
}

int delay_arc(string flt_temp) {
	return stoi(flt_temp.substr(flt_temp.find("_") + 1, flt_temp.npos));
}


int result_ga(string ga) {
	auto it = find(namesga.begin(), namesga.end(), ga);
	if (it != namesga.end()) {
		int pos = distance(namesga.begin(), it);
		return results_namesga[pos];
	}
	else
		return -1;
}

int result_ga_gas(int cnf, string ga) {
	auto it = find_if(gas[cnf].begin(), gas[cnf].end(), [ga](const struct_gas& a) { return a.name == ga; });
	if (it != gas[cnf].end()) {
		int pos = distance(gas[cnf].begin(), it);
		return gas[cnf][pos].result;
	}
	else
		return -1;
}

int busca_cfg(string tmp_model, string tmp_family, string tmp_conf) {
	string buscado = tmp_model + tmp_family + tmp_conf;
	auto it = find_if(cfgs.begin(), cfgs.end(), [buscado](const config_struct& a) { return (a.model + a.family + a.cfg) == buscado; });
	if (it != cfgs.end())
		return distance(cfgs.begin(), it);
	else {
		config_struct tmp_cfgacft;
		vector <string> sep = split(tmp_conf, '/');
		tmp_cfgacft.model = tmp_model;
		tmp_cfgacft.id = cfgs.size();
		tmp_cfgacft.family = tmp_family;
		tmp_cfgacft.cfg = tmp_conf;
		tmp_cfgacft.first = stoi(sep[0]);
		tmp_cfgacft.bus = stoi(sep[1]);
		tmp_cfgacft.eco = stoi(sep[2]);
		tmp_cfgacft.ass = tmp_cfgacft.first + tmp_cfgacft.bus + tmp_cfgacft.eco;
		tmp_cfgacft.mnt_count = 0;
		cfgs.push_back(tmp_cfgacft);
		return cfgs.size() - 1;
	}
}

int find_cnf(string id) {
	return acfts[distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [id](const aircraft_struct& a) { return a.id == id; }))].cfg;
}

bool comp_timee2(const aepflts& a, const aepflts& b) { return a.hora < b.hora; }

bool comp_arrv(const flight_struct& a, const flight_struct& b) { return a.arrv < b.arrv; }

string esps(int cont) {
	string si("");
	for (int i = 0; i<cont; i++)
		si = si + " ";
	return si;
}


void read_instance(string caminho) {
	vector < int >  infs;
	ofstream cout((char*)nome_output.c_str());
	string line, str, line2;
	int teste = 1;
	
	// aircraft file
	string tmp_conf, tmp_model, tmp_family;
	int id_cfgacft;
	int n_acfts = 0;
	str = caminho + "/aircraft.csv";
	ifstream arq_acft((char*)str.c_str(), ios::in);

	for (n_acfts = 0; getline(arq_acft, line); ++n_acfts) {
		if (line[1] == '#')
			break;
	}
	n_acfts--;
	arq_acft.clear();
	arq_acft.seekg(0, ios::beg);
	for (int i = 0; i<n_acfts; i++) {
		//if (line[1] == '#') break;
		aircraft_struct tmp_acft;
		arq_acft >> tmp_acft.id >> tmp_model >> tmp_family >> tmp_conf >> tmp_acft.fh >> tmp_acft.op_cost >> tmp_acft.tat >> tmp_acft.tt >> tmp_acft.airp >> tmp_acft.mant;
		tmp_acft.cfg = busca_cfg(tmp_model, tmp_family, tmp_conf);
		acfts.push_back(tmp_acft);
		cfgs[tmp_acft.cfg].tat = tmp_acft.tat;
		cfgs[tmp_acft.cfg].tt = tmp_acft.tt;

	}
	arq_acft.close();
	cout << "aircraft file" << endl;
	infs.push_back(acfts.size());
	infs.push_back(cfgs.size());
	
	//flighths file
	max_fln = -999;
	string st1, st2;
	int n_flights;
	str = caminho + "/flights.csv";
	ifstream arq_flights((char*)str.c_str(), ios::in);
	for (n_flights = 0; getline(arq_flights, line); ++n_flights) {
		if (line[1] == '#')
			break;
	}
	n_flights--;
	arq_flights.clear();
	arq_flights.seekg(0, ios::beg);
	for (int i = 0; i<n_flights; i++) {
		//if (line[1] == '#') break;
		flight_struct tmp_flt;
		arq_flights >> tmp_flt.nflight >> tmp_flt.org >> tmp_flt.dest >> st1 >> st2 >> tmp_flt.lastf;
		tmp_flt.dept = hora_int(st1);
		tmp_flt.arrv = hora_int(st2);
		tmp_flt.mant = false;
		tmp_flt.thr_pre = false;
		tmp_flt.nextf = 0;
		tmp_flt.result = 0;

		flts.push_back(tmp_flt);
		if (tmp_flt.nflight>max_fln) max_fln = tmp_flt.nflight; // identify de max flight number
	}
	arq_flights.close();
	cout << "flight file" << endl;
	infs.push_back(flts.size());
	

	// rotation file
	vector <string> aux_acfts;
	int n_rot;
	min_date = 999;
	max_date = -999;
	str = caminho + "/rotations.csv";
	ifstream arq_rot((char*)str.c_str(), ios::in);
	for (n_rot = 0; getline(arq_rot, line); ++n_rot) {
		if (line[1] == '#')
			break;
	}
	string priorcfg = "ZYK";
	int priordis = 9999;
	int priorfltpos = 9999;
	n_rot--;
	arq_rot.clear();
	arq_rot.seekg(0, ios::beg);
	vector < string > orgsacft; // vector with input acfs alread counted on
	for (int i = 0; i<n_rot; i++) {
		//if (line[1] == '#') break;
		rotation_struct tmp_rot;
		arq_rot >> tmp_rot.nflight >> tmp_rot.str_data >> tmp_rot.id_acft;
		//cout << tmp_rot.nflight << " " << tmp_rot.str_data << " " << tmp_rot.id_acft << endl;
		tmp_rot.d_flight = stoi(tmp_rot.str_data.substr(0, 2));
		tmp_rot.flid = to_string(tmp_rot.d_flight) + "d" + to_string(tmp_rot.nflight);

		string tmpcfg = tmp_rot.id_acft;
		int dis = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [tmpcfg](const aircraft_struct& a) { return a.id == tmpcfg; }));

		tmp_rot.cfg = acfts[dis].cfg;
		int buscado = tmp_rot.nflight;
		int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [buscado](const flight_struct& a) { return a.nflight == buscado; }));
		flts[flt_pos].dates.push_back(tmp_rot.d_flight); // input dates in flight vector

		aux_acfts.push_back(tmp_rot.id_acft);
		rots.push_back(tmp_rot);




		// verify if acft is comming in the network
		if (tmpcfg != priorcfg) {
			if (find(orgsacft.begin(), orgsacft.end(), tmpcfg) == orgsacft.end()) {
				flts[flt_pos].inflight = true; //no longer used
				orgsacft.push_back(tmpcfg);
			}

			if (acfts[dis].t1 != 99999) {
				if (flts[flt_pos].dept < acfts[dis].t1)
					acfts[dis].t1 = flts[flt_pos].dept;
			}
			else
				acfts[dis].t1 = flts[flt_pos].dept;
		}
		if (priordis != 9999)
			if (acfts[priordis].t1 != 99999)
				if (flts[priorfltpos].arrv > acfts[priordis].t2)
					acfts[priordis].t2 = flts[priorfltpos].arrv;
				else
					acfts[priordis].t2 = flts[priorfltpos].arrv;


		priorcfg = tmpcfg;
		priordis = dis;
		priorfltpos = flt_pos;

		// get min and max dates of the schedule
		if (tmp_rot.d_flight < min_date) min_date = tmp_rot.d_flight;
		if (tmp_rot.d_flight > max_date) max_date = tmp_rot.d_flight;

	}
	arq_rot.close();
	cout << "rotation file" << endl;


	// count the number of flight legs each id_acft fly
	for (int i = 0; i<acfts.size(); i++) {
		string buscado = acfts[i].id;
		acfts[i].nflights = count_if(rots.begin(), rots.end(), [buscado](const rotation_struct& a) { return a.id_acft == buscado; });
	}

	//adjust flight vector
	vector < flight_struct > tempflts;

	for (int i = 0; i < flts.size(); i++)
		// deal with thru flights
		if (flts[i].lastf != 0) {
			int buscado = flts[i].lastf;
			int fltpos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [buscado](const flight_struct& a) { return a.nflight == buscado; }));
			flts[fltpos].thr_pre = true;
			//flts[fltpos].TAT_apl = cfgs[ flts[fltpos].cfg].tt;
			flts[fltpos].nextf = flts[i].nflight;
		}



	for (int i = 0; i < flts.size(); i++) {
		sort(flts[i].dates.begin(), flts[i].dates.end());
		flts[i].flid = to_string(flts[i].dates[0]) + "d" + to_string(flts[i].nflight);
		string flt_temp = flts[i].flid;
		int conf = distance(rots.begin(), find_if(rots.begin(), rots.end(), [flt_temp](const rotation_struct& a) { return a.flid == flt_temp; }));

		flts[i].cfg = rots[conf].cfg;
		flts[i].org_acft = rots[conf].id_acft;
		flts[i].final_acft = flts[i].org_acft;
		// create flight records for flights with the same flight number in different dates
		if (flts[i].dates.size() > 1) {
			//sort(flts[i].dates.begin(),flts[i].dates.end());
			//flts[i].flid = to_string(flts[i].dates[0]) + "d" + to_string(flts[i].nflight);
			// creation starts in the second date. not in the first
			for (int j = 1; j < flts[i].dates.size(); j++) {
				flight_struct tempflt = flts[i];
				tempflt.flid = to_string(flts[i].dates[j]) + "d" + to_string(flts[i].nflight);
				tempflt.dept = tempflt.dept + 1440 * (flts[i].dates[j] - min_date);
				tempflt.arrv = tempflt.arrv + 1440 * (flts[i].dates[j] - min_date);
				string flt_temp = tempflt.flid;
				int conf = distance(rots.begin(), find_if(rots.begin(), rots.end(), [flt_temp](const rotation_struct& a) { return a.flid == flt_temp; }));
				int cnf = rots[conf].cfg;
				tempflt.cfg = cnf;
				tempflt.org_acft = rots[conf].id_acft;
				tempflt.final_acft = tempflt.org_acft;
				tempflts.push_back(tempflt);
			}
		}
	}
	for (int i = 0; i<tempflts.size(); i++)
		flts.push_back(tempflts[i]);

	// Applicable TAT
	for (int i = 0; i < flts.size(); i++)
		if (flts[i].thr_pre)
			flts[i].TAT_apl = cfgs[flts[i].cfg].tt;
		else
			flts[i].TAT_apl = cfgs[flts[i].cfg].tat;

	//config file linha 1
	string st3, st4;
	str = caminho + "/config.csv";
	ifstream arq_cfg((char*)str.c_str(), ios::in);
	arq_cfg >> st1 >> st2 >> st3 >> st4;
	recovery_time.d1 = stoi(st1.substr(0, 2));
	recovery_time.t1 = hora_int(st2);
	recovery_time.d2 = stoi(st3.substr(0, 2));
	recovery_time.t2 = hora_int(st4);
	recovery_time.beginning = (recovery_time.d1 - min_date) * 1440 + recovery_time.t1;
	recovery_time.end = (recovery_time.d2 - min_date) * 1440 + recovery_time.t2;
	

	for (size_t i = 0; i < 4; i++)
		arq_cfg >> st3;
	arq_cfg >> recovery_time.P1 >> recovery_time.P2 >> recovery_time.P3;
	arq_cfg >> recovery_time.a >> recovery_time.b >> recovery_time.c;

	//airports file
	str = caminho + "/airports.csv";
	int naep, tmp_dep, tmp_arr, h1, h2;
	int aep_id = 0;
	ifstream arq_aerp((char*)str.c_str(), ios::in);
	for (naep = 0; getline(arq_aerp, line); ++naep)
		if (line[1] == '#') break;
	naep--;
	arq_aerp.clear();
	arq_aerp.seekg(0, ios::beg);
	for (int i = 0; i<naep; i++) {
		getline(arq_aerp, line);
		//if (line[1] == '#') break;
		airport_struct tmp_aerp;
		vector <string> sep = split(line, ' ');
		int n_periods = (sep.size() - 1) / 4;
		tmp_aerp.id = i;
		tmp_aerp.nome = sep[0];
		int col = 0;
		for (int j = 0; j<n_periods; j++) {
			col = 4 * j + 1;
			tmp_dep = stoi(sep[col]);
			tmp_arr = stoi(sep[col + 1]);
			h1 = stoi(sep[col + 2].substr(0, 2));
			h2 = stoi(sep[col + 3].substr(0, 2));
			if (h2 == 0) h2 = 24;
			vector < int > temp_caps;
			temp_caps.push_back(tmp_dep);
			temp_caps.push_back(tmp_arr);
			for (int k = h1; k<h2; k++)
				tmp_aerp.slot_lim.push_back(temp_caps);
		}
		tmp_aerp.org_lim = tmp_aerp.slot_lim;
		aerps.push_back(tmp_aerp);
	}
	arq_aerp.close();
	cout << "airport file" << endl;
	infs.push_back(aerps.size());
	
	
	
	//position file
	str = caminho + "/position.csv";
	ifstream arq_pos((char*)str.c_str(), ios::in);
	for (naep = 0; getline(arq_pos, line); ++naep) {
		if (line[1] == '#') break;
	}
	naep--;
	arq_pos.clear();
	arq_pos.seekg(0, ios::beg);
	for (int i = 0; i<naep; i++) {
		getline(arq_pos, line);
		//if (line[1] == '#') break;
		vector <string> sep = split(line, ' ');
		int n_periods = (sep.size() - 2) / 3;
		int col = 0;
		string buscado = sep[0];
		int aep = distance(aerps.begin(), find_if(aerps.begin(), aerps.end(), [buscado](const airport_struct& a) { return a.nome == buscado; }));

		for (int p = 0; p < cfgs.size(); p++)
			aerps[aep].acft_pos.push_back(col);

		for (int j = 0; j<n_periods; j++) {
			col = 3 * j + 1;
			vector <int> tempv;
			buscado = sep[col] + sep[col + 1];
			int id_cfg = distance(cfgs.begin(), find_if(cfgs.begin(), cfgs.end(), [buscado](const config_struct& a) { return (a.model + a.cfg) == buscado; }));
			tempv.push_back(id_cfg);
			tempv.push_back(stoi(sep[col + 2]));
			aerps[aep].acft_pos[id_cfg] += stoi(sep[col + 2]);
			//tempvv.push_back(tempv);
		}
		//aerps[aep].acft_pos=tempvv;
	}
	arq_pos.close();
	cout << "position file" << endl;

	// count and store flights of each configuration      
	for (int i = 0; i<rots.size(); i++) {
		string buscado = rots[i].id_acft;
		int acf = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [buscado](const aircraft_struct& a) { return a.id == buscado; }));
		int cfg = acfts[acf].cfg;
		cfgs[cfg].fltids.push_back(rots[i].flid);
		string ibuscado = rots[i].flid;
		int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [ibuscado](const flight_struct& a) { return a.flid == ibuscado; }));
		flts[flt_pos].cfg = cfg;
	}

	//count number input aircrafts by type in each airport
	int nacfts;
	for (int i = 0; i<aerps.size(); i++)
		for (int j = 0; j<cfgs.size(); j++) {
			string buscado = aerps[i].nome + to_string(j);
			nacfts = count_if(acfts.begin(), acfts.end(), [buscado](const aircraft_struct& a) { return (a.airp + to_string(a.cfg)) == buscado; });
			aerps[i].in_acfts.push_back(nacfts);
			vector < string > temp_ids;
			if (nacfts>0)
				for (int k = 0; k<acfts.size(); k++) {
					string busc2 = acfts[k].airp + to_string(acfts[k].cfg);
					if (busc2 == buscado)
						temp_ids.push_back(acfts[k].id);
				}
			aerps[i].id_in_acfts.push_back(temp_ids);
		}


	//create maintenance virtual flights
	int mntnum = max_fln + 100;
	int cont_mnt = 0;
	vector<aircraft_struct>::iterator it;
	for (it = acfts.begin(); it != acfts.end(); ++it)
		if (it->mant != "NULL") {
			//update flight vector
			vector <string> sep = split(it->mant, '-');
			flight_struct tmp_mnt;
			tmp_mnt.mant = true;
			tmp_mnt.nflight = mntnum;
			tmp_mnt.org = sep[0];
			tmp_mnt.dest = sep[0];
			tmp_mnt.lastf = 0;
			tmp_mnt.dept = (stoi(sep[1].substr(0, 2)) - recovery_time.d1) * 1440 + hora_int(sep[2]);
			tmp_mnt.arrv = (stoi(sep[3].substr(0, 2)) - recovery_time.d1) * 1440 + hora_int(sep[4]);
			tmp_mnt.flid = to_string(stoi(sep[1].substr(0, 2))) + "d" + to_string(mntnum);
			tmp_mnt.cfg = it->cfg;
			tmp_mnt.org_acft = it->id;
			flts.push_back(tmp_mnt);

			//update rotation vector
			rotation_struct tmp_rot;
			tmp_rot.nflight = mntnum;
			tmp_rot.d_flight = stoi(sep[1].substr(0, 2));
			tmp_rot.id_acft = it->id;
			tmp_rot.cfg = it->cfg;
			tmp_rot.flid = to_string(tmp_rot.d_flight) + "d" + to_string(mntnum);
			rots.push_back(tmp_rot);

			//update aircraft configurations vecto
			cfgs[it->cfg].mnt_count++;
			cfgs[it->cfg].fltids.push_back(tmp_rot.flid);

			max_fln_mnt = mntnum; // set last maintenance number

			cont_mnt++;
			mntnum++;
		}
	
	auto max_arrv_it = max_element(flts.begin(), flts.end(), comp_arrv);
	int max_arrv = max_arrv_it->arrv;
	cout << "max arrv " << max_arrv << " " << int_hora(max_arrv) << " " << max_arrv_it->flid;
	if (recovery_time.end < max_arrv)
		recovery_time.end = max_arrv;

	// create airport slot capacity vector
	int max_arr_hour = (max_arrv + noptions * min_opt)/60;
	for (size_t j = 0; j < aerps.size(); j++)
	{	
		airpcap_struct tempairca;
		for (size_t i = 0; i < max_arr_hour; i++) {
			tempairca.depcap.push_back(aerps[j].slot_lim[i % 24][0]);
			tempairca.arrvcap.push_back(aerps[j].slot_lim[i % 24][1]);
		}
		tempairca.deporgcap = tempairca.depcap;
		tempairca.arrvorgcap = tempairca.arrvcap;
		airpcap.push_back(tempairca);
	}
	



	infs.push_back(cont_mnt);
	// altflights file
	int n_alts = 0;
	str = caminho + "/alt_flights.csv";
	ifstream arq_altflt((char*)str.c_str(), ios::in);

	for (n_alts = 0; getline(arq_altflt, line); ++n_alts) {
		if (line[1] == '#') break;
	}
	n_alts--;
	arq_altflt.clear();
	arq_altflt.seekg(0, ios::beg);
	for (int i = 0; i<n_alts; i++) {
		//if (line[1] == '#') break;
		altflights_struct tmp_altflt;
		arq_altflt >> tmp_altflt.nflight >> tmp_altflt.str_data >> tmp_altflt.atr;
		tmp_altflt.d_flight = stoi(tmp_altflt.str_data.substr(0, 2));
		tmp_altflt.flid = to_string(tmp_altflt.d_flight) + "d" + to_string(tmp_altflt.nflight);
		if (tmp_altflt.atr > 0) {
			tmp_altflt.atr_int = tmp_altflt.atr / min_opt;
			if ((tmp_altflt.atr % min_opt) > 0)
				tmp_altflt.atr_int++;
			tmp_altflt.flid_pert = tmp_altflt.flid + "_" + to_string(tmp_altflt.atr_int);
		}
		else
			tmp_altflt.atr_int = tmp_altflt.atr;

		int hora_alt = flts[fltps(tmp_altflt.flid)].dept;
		if (hora_alt < pert_time) pert_time = hora_alt;
		if (tmp_altflt.atr > 0)
			flts[fltps(tmp_altflt.flid)].atr = tmp_altflt.atr_int * min_opt;
		else
			flts[fltps(tmp_altflt.flid)].atr = -1;

		altflts.push_back(tmp_altflt);
	}
	arq_altflt.close();
	cout << "altflights file" << endl;
	infs.push_back(altflts.size());

	// alt_airports file
	n_alts = 0;
	str = caminho + "/alt_airports.csv";
	ifstream arq_altarp((char*)str.c_str(), ios::in);
	for (n_alts = 0; getline(arq_altarp, line); ++n_alts) {
		if (line[1] == '#') break;
	}
	n_alts--;
	arq_altarp.clear();
	arq_altarp.seekg(0, ios::beg);
	for (int i = 0; i<n_alts; i++) {
		//if (line[1] == '#') break;
		altslot_struct tmp_altarp;
		string td1, td2, th1, th2;
		arq_altarp >> tmp_altarp.airp >> td1 >> th1 >> td2 >> th2 >> tmp_altarp.dep >> tmp_altarp.arrv;
		tmp_altarp.h1 = ((stoi(td1.substr(0, 2)) - min_date) * 1440 + hora_int(th1)) / 60;
		tmp_altarp.h2 = ((stoi(td2.substr(0, 2)) - min_date) * 1440 + hora_int(th2)) / 60;
		if ((tmp_altarp.h1 * 60) < pert_time) pert_time = (tmp_altarp.h1 * 60);

		string sttemp = tmp_altarp.airp;
		int aeppos = distance(aerps.begin(), find_if(aerps.begin(), aerps.end(), [sttemp](const airport_struct& a) { return a.nome == sttemp; }));

		// Change airport capacity by disturbed one
		for (int hh = tmp_altarp.h1; hh < tmp_altarp.h2; hh++) {
			airpcap[aeppos].depcap[hh] = tmp_altarp.dep;
			airpcap[aeppos].arrvcap[hh] = tmp_altarp.arrv;
		}

		altslots.push_back(tmp_altarp);
	}
	arq_altarp.close();
	cout << "alt_airports file" << endl;
	infs.push_back(altslots.size());
	// alt_aircraft file

	//  create maintenance flights at all the airport of that configuration
	// create maintenance flights at the airport where the acft should be if original rotation were followed
	mntnum += 100;
	n_alts = 0;
	str = caminho + "/alt_aircraft.csv";
	ifstream arq_altacft((char*)str.c_str(), ios::in);
	for (n_alts = 0; getline(arq_altacft, line); ++n_alts) {
		if (line[1] == '#') break;
	}
	n_alts--;
	arq_altacft.clear();
	arq_altacft.seekg(0, ios::beg);
	for (int i = 0; i<n_alts; i++) {
		// if (line[1] == '#') break;
		altacft_struct tmp_altacft;
		string td1, td2, th1, th2;
		arq_altacft >> tmp_altacft.acft_id >> td1 >> th1 >> td2 >> th2;

		tmp_altacft.d1 = stoi(td1.substr(0, 2));
		tmp_altacft.d2 = stoi(td2.substr(0, 2));
		tmp_altacft.t1 = ((tmp_altacft.d1 - min_date) * 1440 + hora_int(th1));
		tmp_altacft.t2 = ((tmp_altacft.d2 - min_date) * 1440 + hora_int(th2));
		string buscado = tmp_altacft.acft_id;
		int acf = distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [buscado](const aircraft_struct& a) { return a.id == buscado; }));
		tmp_altacft.cfg = acfts[acf].cfg;
		if (tmp_altacft.t1 < pert_time) pert_time = tmp_altacft.t1;
		altacft.push_back(tmp_altacft);
		int hora_ant = 0;
		int hora_atual;
		if (alt_acfts) {
			vector < string > flids_temp;


			for (int p = 0; p<aerps.size(); p++)
				if (aerp_cfg(aerps[p].nome, tmp_altacft.cfg)) {

					flight_struct tmp_mnt;
					tmp_mnt.mant = true;
					tmp_mnt.nflight = mntnum;
					tmp_mnt.org = aerps[p].nome;
					tmp_mnt.dest = tmp_mnt.org;
					tmp_mnt.lastf = 0;
					tmp_mnt.dept = tmp_altacft.t1;
					tmp_mnt.arrv = tmp_altacft.t2;
					tmp_mnt.flid = to_string(tmp_altacft.d1) + "d" + to_string(mntnum);
					tmp_mnt.cfg = tmp_altacft.cfg;


					flts.push_back(tmp_mnt);

					//update rotation vector
					rotation_struct tmp_rot;
					tmp_rot.nflight = mntnum;
					tmp_rot.d_flight = tmp_altacft.d1;
					tmp_rot.id_acft = tmp_altacft.acft_id;
					tmp_rot.cfg = tmp_mnt.cfg;
					tmp_rot.flid = tmp_mnt.flid;
					rots.push_back(tmp_rot);

					//update aircraft configurations vecto
					cfgs[tmp_mnt.cfg].mnt_count++;
					cfgs[tmp_mnt.cfg].fltids.push_back(tmp_rot.flid);

					altacft[altacft.size() - 1].flids.push_back(tmp_mnt.flid);

					mntnum++;
				}
		}
	}
	arq_altacft.close();
	cout << "alt_aircraft file" << endl;
	infs.push_back(n_alts);
	cout << endl << "Perturbation Resume:" << endl;
	cout << "Initial Perturbation Time: " << pert_time << endl;
	cout << "delayed/canceled flights: " << altflts.size() << endl;
	cout << "airport slot reduction: " << altslots.size() << endl;
	cout << "aircraft breakdown: " << altacft.size() << endl << endl;

	infs.push_back(count_diferents(aux_acfts));
	inf_inputs.push_back(infs);

	infs.clear();
}

void create_delay_opt(int noptions) {
	for (int i = 0; i<flts.size(); i++)
		for (int j = 0; j<noptions; j++) {
			string flt_temp = flts[i].flid;
			flts[i].kfs.push_back(flts[i].flid + "_" + to_string(j));
			if (flts[i].mant) break; // if a maintenance fligth then only 1 delay option
									 //put it in stand by for a while
			if (flts[i].dept < pert_time) break; //flight whose dep is earlier than beginning of recovery time is unchanged
		}
	max_date += (int)ceil(min_opt*noptions / 60 / 24);
	spam_days = (max_date - min_date + 1);


	/*
	//log creat_delay_opt
	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	cout << endl << "Flight Delay Options" << endl;
	for (int i = 0; i<flts.size(); i++) {
		for (int j = 0; j<flts[i].kfs.size(); j++)
			cout << flts[i].nflight << " " << flts[i].dates.size() << " " << flts[i].kfs[j] << " ";
		cout << endl;
	}
	*/

}

vector < string > arctokeys(string arc) {
	//key1, aep1, key2, aep2
	vector < string > tmp;
	string key1, key2, aep1, aep2;
	if (arc.at(0) != 'g') {
		int pos = arctoflpos(arc);
		int delay = stoi(arc.substr(arc.find("_") + 1, arc.npos));
		int timee;
		// departure key
		int var_time = delay*min_opt;
		int cfg = flts[pos].cfg;
		int time_real = flts[pos].dept + var_time;
		if (flts[pos].mant)
			timee = time_real + cfgs[cfg].tat;
		else
			timee = time_real;
		aep1 = flts[pos].org;
		key1 = aep1 + to_string(timee);
		//arrival key
		int ttat;
		if (flts[pos].thr_pre)
			ttat = cfgs[cfg].tt;
		else
			ttat = cfgs[cfg].tat;

		timee = flts[pos].arrv + ttat + var_time; // increase arrival time with TAT
		aep2 = flts[pos].dest;
		key2 = aep2 + to_string(timee);

	}
	else {
		vector<string> sep = split(arc, '_');
		aep1 = sep[3].substr(0, 3);
		key1 = sep[3];
		if (sep.size() == 6) {
			aep2 = sep[5].substr(0, 3);
			key2 = sep[4] + "_" + sep[5];
		}
		else {
			aep2 = sep[4].substr(0, 3);
			key2 = sep[4];
		}
	}
	tmp.push_back(key1);
	tmp.push_back(aep1);
	tmp.push_back(key2);
	tmp.push_back(aep2);
	return tmp;
}


void create_flt_events() {

	string scfg_transpcom("TranspCom");
	int cfg_transpcom = distance(cfgs.begin(), find_if(cfgs.begin(), cfgs.end(), [scfg_transpcom](const config_struct& a) { return a.model == scfg_transpcom; }));


	for (int i = 0; i < flts.size(); i++) {

		/*string flt_temp = flts[i].flid;
		int conf = distance(rots.begin(),find_if(rots.begin(),rots.end(), [flt_temp] (const rotation_struct& a) { return a.flid==flt_temp; }));
		int cnf = rots[conf].cfg;*/

		int cnf = flts[i].cfg;

		for (int j = 0; j < flts[i].kfs.size(); j++) {

			int delay = stoi(flts[i].kfs[j].substr(flts[i].kfs[j].find("_") + 1, flts[i].kfs[j].npos));

			//departure event
			event_struct tmpdec;
			tmpdec.time_real = flts[i].dept + delay*min_opt;
			if (flts[i].mant)
				tmpdec.timee = tmpdec.time_real + cfgs[flts[i].cfg].tat;
			else
				tmpdec.timee = tmpdec.time_real;

			tmpdec.h_slot = tmpdec.time_real / 60;
			if (tmpdec.h_slot > limit_hour_slot) limit_hour_slot = tmpdec.h_slot;
			tmpdec.local = flts[i].org;
			tmpdec.key_flight = flts[i].kfs[j];
			tmpdec.type = 1;
			tmpdec.key = tmpdec.local + to_string(tmpdec.timee);
			tmpdec.cnfg = cnf;
			tmpdec.nflight_ant = flts[i].lastf;
			tmpdec.nflight_pos = flts[i].nextf;
			if (flts[i].lastf > 0)
				tmpdec.tt = cfgs[flts[i].cfg].tt;
			else
				tmpdec.tt = cfgs[flts[i].cfg].tat;

			//if (j==0) // only the undelayed flight can be an input event
			//    tmpdec.inflt = flts[i].inflight;

			if (cnf != cfg_transpcom)
				evts.push_back(tmpdec);

			//arrival event
			event_struct tmparrv;

			int ttat;

			if (flts[i].thr_pre)
				ttat = cfgs[flts[i].cfg].tt;
			else
				ttat = cfgs[flts[i].cfg].tat;

			//ttat = cfgs[flts[i].cfg].tt;

			//ttat = 0;


			tmparrv.timee = flts[i].arrv + ttat + delay*min_opt; // increase arrival time with TAT
			tmparrv.time_real = tmparrv.timee - ttat; // increase arrival time with TAT
			tmparrv.h_slot = tmparrv.time_real / 60;
			if (tmparrv.h_slot > limit_hour_slot) limit_hour_slot = tmparrv.h_slot;
			tmparrv.local = flts[i].dest;
			tmparrv.key_flight = flts[i].kfs[j];
			tmparrv.type = 0;
			tmparrv.key = tmparrv.local + to_string(tmparrv.timee);
			tmparrv.cnfg = cnf;
			//tmparrv.inflt = false;

			if (cnf != cfg_transpcom)
				evts.push_back(tmparrv);
		}
	}
	
	
	for ( int i=0; i < cfgs.size(); i++)
		for (int j = 0; j < aerps.size(); j++) {
			event_struct tmparrv;
			tmparrv.cnfg = i;
			tmparrv.timee = recovery_time.end + cfgs[i].tat;
			tmparrv.local = aerps[j].nome;
			tmparrv.key = tmparrv.local + to_string(tmparrv.timee);
			tmparrv.key_flight = "END_REVOVERY";
			tmparrv.type = 0;
			if (i != cfg_transpcom)
				evts.push_back(tmparrv);
		} 
		
}

bool comp_timee(const activity_struct& a, const activity_struct& b) { return a.timee < b.timee; }



void create_network() {

	int no_pos;

	//initialize nos vector
	for (int j = 0; j<cfgs.size(); j++) {
		vector < vector < activity_struct > > temp1_nos;
		vector < vector < string > > temp_names;
		nos.push_back(temp1_nos);
		for (int i = 0; i<aerps.size(); i++) {
			vector < activity_struct > temp_no;
			vector < string > temp_names2;
			nos[j].push_back(temp_no);
		}
	}

	// complete Input/Output flight arc list
	for (int i = 0; i<evts.size(); i++) {
		// look for a key=[local_time] node
		int cfg_pos = evts[i].cnfg;
		string buscado = evts[i].local;
		int aep = distance(aerps.begin(), find_if(aerps.begin(), aerps.end(), [buscado](const airport_struct& a) { return a.nome == buscado; }));
		activity_struct temp_no;
		temp_no.conf = cfg_pos;
		temp_no.key = evts[i].key;
		temp_no.local = evts[i].local;
		temp_no.timee = evts[i].timee;
		temp_no.in_acft = 0;
		temp_no.tat = cfgs[cfg_pos].tat;
		//if (evts[i].inflt)
		//    temp_no.in_acft=1;
		buscado = evts[i].key;
		auto it = find_if(nos[cfg_pos][aep].begin(), nos[cfg_pos][aep].end(), [buscado](const activity_struct& a) { return a.key == buscado; });
		//if there is not a key the create it and get its position
		if (it != nos[cfg_pos][aep].end())
			no_pos = distance(nos[cfg_pos][aep].begin(), it);
		else {
			if (evts[i].key_flight != "END_REVOVERY") {
				nos[cfg_pos][aep].push_back(temp_no);
				no_pos = nos[cfg_pos][aep].size() - 1;
			}
			else
				if (nos[cfg_pos][aep].size() > 0) {
					nos[cfg_pos][aep].push_back(temp_no);
					no_pos = nos[cfg_pos][aep].size() - 1;
				}
		}
		// fill up inflight or outflight list
		if (evts[i].type) {
			nos[cfg_pos][aep][no_pos].outfltarc.push_back(evts[i].key_flight);
			nos[cfg_pos][aep][no_pos].voo_ant_list.push_back(evts[i].nflight_ant);
			nos[cfg_pos][aep][no_pos].voo_pos_list.push_back(evts[i].nflight_pos);

			arcflt_struct temp_strut;
			temp_strut.flidpos = arctoflpos(evts[i].key_flight);
			temp_strut.opc = arctoopc(evts[i].key_flight);
			nos[cfg_pos][aep][no_pos].outfltarc_int.push_back(temp_strut);
		}
		else 
			if (evts[i].key_flight != "END_REVOVERY") {
				nos[cfg_pos][aep][no_pos].infltarc.push_back(evts[i].key_flight);
				arcflt_struct temp_strut;
				temp_strut.flidpos = arctoflpos(evts[i].key_flight);
				temp_strut.opc = arctoopc(evts[i].key_flight);
				nos[cfg_pos][aep][no_pos].infltarc_int.push_back(temp_strut);
			}
	}

	//sort each airport line by time and input number of acfts in the beggining of recovery period
	for (int j = 0; j < cfgs.size(); j++)
		for (int i = 0; i < aerps.size(); i++)
			if (nos[j][i].size() > 0) {

				sort(nos[j][i].begin(), nos[j][i].end(), comp_timee);
				// Create a first Node to be Input Aircraft
				activity_struct temp_no;
				temp_no.conf = j;
				temp_no.local = nos[j][i][0].local;
				temp_no.timee = nos[j][i][0].timee - 10;
				temp_no.key = temp_no.local + to_string(temp_no.timee);
				nos[j][i].insert(nos[j][i].begin(), temp_no);
				nos[j][i][0].in_acft = aerps[i].in_acfts[j];
				//Insert output nodes
				activity_struct temp_no2;
				temp_no2 = nos[j][i][nos[j][i].size() - 1]; // copy prior node
				temp_no2.timee = temp_no2.timee + 10; // output node 10 min later prior node
				//temp_no2.timee = recovery_time.end; // output node 10 min later prior node
				temp_no2.key = "OUT_" + temp_no2.local + to_string(temp_no2.timee);
				temp_no2.outfltarc.clear();
				temp_no2.infltarc.clear();
				temp_no2.outfltarc_int.clear();
				temp_no2.infltarc_int.clear();
				nos[j][i].push_back(temp_no2);

			}
	

	int h = 0;
	// create ground arcs
	int ult_j, count_ga = 0;
	for (int p = 0; p<cfgs.size(); p++)
		for (int i = 0; i<aerps.size(); i++)
			if (nos[p][i].size()>0) {
				for (int j = 0; j<nos[p][i].size() - 1; j++) {
					string ga = nos[p][i][j].key + "_" + nos[p][i][j + 1].key;

					ga = "ga_" + to_string(count_ga) + "_" + to_string(p) + "_" + ga;

					nos[p][i][j].outgrdarc.push_back(ga);
					nos[p][i][j + 1].ingrdarc.push_back(ga);
					nos[p][i][j].outgrdarc_int.push_back(h);
					nos[p][i][j + 1].ingrdarc_int.push_back(h);


					namesga.push_back(ga);
					conf_namesga.push_back(p);
					results_namesga.push_back(-9);
					acfts_namesga.push_back(-9);
					ult_j = j;
					count_ga++;
					h++;
				}
				//namesga[ult_j] += "_OUT";
				count_ga = 0;
			}



	for (int i = 0; i < cfgs.size(); i++) {
		vector < struct_gas >  tempgas;
		gas.push_back(tempgas);
	}
}

void result_nos() {
	bool saida = false;
	for (int p = 0; p<lista_cfg.size(); p++) {
		int it_cfg = lista_cfg[p];
		for (int i = 0; i<nos[it_cfg].size(); i++)
			for (int k = 0; k<nos[it_cfg][i].size(); k++) {
				saida = false;
				for (int j = 0; j<nos[it_cfg][i][k].infltarc.size(); j++) {
					string flt_temp = nos[it_cfg][i][k].infltarc[j];
					if (arc_result(flt_temp)) {
						saida = true;
						break;
					}
				}
				if (!saida)
					for (int j = 0; j<nos[it_cfg][i][k].ingrdarc.size(); j++)
						if (result_ga_gas(it_cfg, nos[it_cfg][i][k].ingrdarc[j]) > 0) {
							saida = true;
							break;
						}

				if (!saida)
					for (int j = 0; j<nos[it_cfg][i][k].outfltarc.size(); j++) {
						string flt_temp = nos[it_cfg][i][k].outfltarc[j];
						if (arc_result(flt_temp)) {
							saida = true;
							break;
						}
					}

				if (!saida)
					for (int j = 0; j<nos[it_cfg][i][k].outgrdarc.size(); j++)
						if (result_ga_gas(it_cfg, nos[it_cfg][i][k].outgrdarc[j]) > 0) {
							saida = true;
							break;
						}
				nos[it_cfg][i][k].tout_out = saida;
			}
	}
}

int le_r1() {

	//Leitura de Variaveis X 

	//grep 'x' A01_6088570.sol | gawk '{ if ($2==1) print $1 }' | gawk -F_ '{ print $2,$3,$4 }' > x_NB.txt

	//grep 'y' A01_6088570.sol | gawk '{ if ($2==1) print $1 }' | gawk -F_ '{ print $2,$3 }' > y_NB.txt

	//grep 'ga' A01_6088570.sol | gawk '{ if ($2>1) print $1,$2 }' > ga_NB.txt



	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	string arqres = "./Results/" + nome_instancia + "/results_" + nome_instancia + "_" + to_string(noptions) + ".txt";
	ofstream res_file((char *)arqres.c_str());

	string line, str("./Results/"), line2;
	int result;

	str = str + nome_instancia + "/" + nome_instancia + ".sol";
	string str2 = "./Results/" + nome_instancia + "/res_atrib.txt";

	string comadx = "gawk '{ if ($2!=0) print $0 }' " + str + " > " + str2;
	int i = system((char*)comadx.c_str());

	for (int i = 0; i<results_namesga.size(); i++) results_namesga[i] = 0;

	ifstream arq_acft((char*)str2.c_str());
	while (getline(arq_acft, line)) {
		//res_file << line << endl;
		vector <string> sep = split(line, ' ');
		//res_file << sep[0] << " " << sep[1] << endl;
		if (sep[0].at(0) == '#')
			result_1 = stoi(sep[sep.size() - 1]);
		else if (sep[0].at(0) == 'x') {
			vector <string> sep2 = split(sep[0], '_');
			string fltr = sep2[2];
			int fltr_dl = stoi(sep2[3]);
			int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [fltr](const flight_struct& a) { return a.flid == fltr; }));
			if (stoi(sep[sep.size() - 1])>0) {
				flts[flt_pos].result = fltr_dl;
				flts[flt_pos].org_result = fltr_dl;
				//res_file << fltr << " " << fltr_dl << endl;
				flts[flt_pos].cost = fltr_dl * minute_cost * min_opt;
			}
			else
				if (flid_alt_acft(flts[flt_pos].flid))
					flts[flt_pos].result = -1;
		}
		else if (sep[0].at(0) == 'y') {
			if (stoi(sep[sep.size() - 1])>0) {

				vector <string> sep2 = split(sep[0], '_');

				string fltr = sep2[2];

				int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [fltr](const flight_struct& a) { return a.flid == fltr; }));
				flts[flt_pos].result = -1;
				flts[flt_pos].org_result = -1;
				flts[flt_pos].cost = cancel_cost;
			}
		}
		else if (sep[0].at(0) == 'g') {
			string flt_temp = sep[0];
			int flt_pos = distance(namesga.begin(), find_if(namesga.begin(), namesga.end(), [flt_temp](const string& a) { return a == flt_temp; }));
			results_namesga[flt_pos] = stoi(sep[sep.size() - 1]);
			int it_cfg = conf_namesga[flt_pos];
			if (results_namesga[flt_pos]>0) {
				struct_gas tempgas;
				tempgas.name = flt_temp;
				tempgas.result = results_namesga[flt_pos];
				gas[it_cfg].push_back(tempgas);
			}
		}
		org_results_namesga = results_namesga;
	}
	result_nos(); // atualiza nos com o resultado geral

				  //show results
	cout << endl << endl << "Flights file Results" << endl;
	res_file << nome_instancia << endl;
	res_file << "objetivo: " << endl << result_1 << endl;
	res_file << "voo  config delay_opt cost" << endl;
	for (int i = 0; i<flts.size(); i++)
		if (find(lista_cfg.begin(), lista_cfg.end(), flts[i].cfg) != lista_cfg.end())
			if (cfgs[flts[i].cfg].model != "TranspCom")
				res_file << flts[i].flid << " " << cfgs[flts[i].cfg].model << " " << flts[i].result << endl;
	for (int i = 0; i<conf_namesga.size(); i++)
		for (int j = 0; j<lista_cfg.size(); j++)
			if (conf_namesga[i] == lista_cfg[j])
				res_file << namesga[i] << " " << conf_namesga[i] << " " << results_namesga[i] << endl;
	arq_acft.close();
	return result_1;
}


int le_rout_ass() {

	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	string arqres = "./Results/" + nome_instancia + "/results_" + nome_instancia + "_" + to_string(noptions) + ".txt";
	ofstream res_file((char *)arqres.c_str());

	string line, str("./Results/"), line2;
	int result;

	str = str + nome_instancia + "/" + nome_instancia + ".sol";
	string str2 = "./Results/" + nome_instancia + "/res_atrib.txt";

	string comadx = "gawk '{ if ($2!=0) print $0 }' " + str + " > " + str2;
	int i = system((char*)comadx.c_str());

	for (int i = 0; i<results_namesga.size(); i++) results_namesga[i] = 0;

	ifstream arq_acft((char*)str2.c_str());
	while (getline(arq_acft, line)) {
		//res_file << line << endl;
		vector <string> sep = split(line, ' ');
		//res_file << sep[0] << " " << sep[1] << endl;
		if (sep[0].at(0) == '#')
			result_1 = stoi(sep[sep.size() - 1]);
		else if (sep[0].at(0) == 'x') {
			vector <string> sep2 = split(sep[0], '_');
			string fltr = sep2[2];
			int fltr_dl = stoi(sep2[3]);
			int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [fltr](const flight_struct& a) { return a.flid == fltr; }));
			if (stoi(sep[sep.size() - 1])>0) {
				flts[flt_pos].result = fltr_dl;
				flts[flt_pos].org_result = fltr_dl;
				flts[flt_pos].cost = fltr_dl * minute_cost * min_opt;
				int acf_pos = stoi(sep2[4]);
				flts[flt_pos].final_acft = acfts[acf_pos].id;
			}
			else
				if (flid_alt_acft(flts[flt_pos].flid))
					flts[flt_pos].result = -1;
		}
		else if (sep[0].at(0) == 'y') {
			if (stoi(sep[sep.size() - 1])>0) {

				vector <string> sep2 = split(sep[0], '_');

				string fltr = sep2[2];

				int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [fltr](const flight_struct& a) { return a.flid == fltr; }));
				flts[flt_pos].result = -1;
				flts[flt_pos].org_result = -1;
				flts[flt_pos].cost = cancel_cost;
			}
		}
		else if (sep[0].at(0) == 'g') {
			string flt_temp = sep[0].substr(0, sep[0].find_last_of("_"));
			int flt_pos = distance(namesga.begin(), find_if(namesga.begin(), namesga.end(), [flt_temp](const string& a) { return a == flt_temp; }));
			results_namesga[flt_pos] = stoi(sep[sep.size() - 1]);
			int it_cfg = conf_namesga[flt_pos];
			if (results_namesga[flt_pos]>0) {
				acfts_namesga[flt_pos] = stoi(sep[0].substr(sep[0].find_last_of("_") + 1, sep[0].npos));
				struct_gas tempgas;
				tempgas.name = flt_temp;
				tempgas.result = results_namesga[flt_pos];
				gas[it_cfg].push_back(tempgas);
			}
		}
		org_results_namesga = results_namesga;
	}
	//result_nos(); // atualiza nos com o resultado geral

	//show results
	cout << endl << endl << "Flights file Results" << endl;
	res_file << nome_instancia << endl;
	res_file << "objetivo: " << endl << result_1 << endl;
	res_file << "voo  config delay_opt cost" << endl;
	for (int i = 0; i<flts.size(); i++)
		if (find(lista_cfg.begin(), lista_cfg.end(), flts[i].cfg) != lista_cfg.end())
			if (cfgs[flts[i].cfg].model != "TranspCom")
				res_file << flts[i].flid << " " << cfgs[flts[i].cfg].model << " " << flts[i].result << endl;
	//for (int i=0;i<conf_namesga.size();i++)
	//    for (int j=0;j<lista_cfg.size();j++)
	//        if (conf_namesga[i]==lista_cfg[j])
	//            res_file << namesga[i] << " " << conf_namesga[i] << " " << results_namesga[i] << endl;
	arq_acft.close();
	res_file.close();
	return result_1;
}


int le_r2() {

	string arqres = "./Results/" + nome_instancia + "/results_" + nome_instancia + "_" + to_string(noptions) + ".txt";
	ofstream res_file((char *)arqres.c_str());
	ofstream cout((char*)nome_output.c_str(), std::fstream::app);

	string str = "./Results/" + nome_instancia + "/";

	str = str + nome_instancia + ".sol";

	string str_x = str + "x_NB.txt";
	string str_y = str + "y_NB.sol";
	string str_g = str + "ga_NB.sol";

	string comadx = "grep 'x' " + str + " | gawk '{ if ($2==1) print $1 }' | gawk -F_ '{ print $2,$3,$4 }' > " + str_x;
	int i = system((char*)comadx.c_str());

	string comady = "grep 'y' " + str + " | gawk '{ if ($2==1) print $1 }' | gawk -F_ '{ print $2,$3 }' > " + str_y;
	i = system((char*)comady.c_str());

	string comadga = "grep 'ga' " + str + " | gawk '{ if ($2!=0) print $1,$2 }' | gawk -F_ '{ print $3,$0 }' > " + str_g;


	i = system((char*)comadga.c_str());

	int n_acfts;
	string line;

	ifstream arq_acft1((char*)str_x.c_str());
	for (n_acfts = 0; getline(arq_acft1, line); ++n_acfts) {
		if (line[1] == '#')
			break;
	}
	n_acfts--;
	arq_acft1.clear();
	arq_acft1.seekg(0, ios::beg);
	for (int i = 0; i<n_acfts; i++) {
		int cnf, result;
		string fltemp;
		arq_acft1 >> cnf >> fltemp >> result;
		int flpos = flidtoflpos(fltemp);
		flts[flpos].result = result;
		flts[flpos].org_result = result;
		flts[flpos].cost = result * minute_cost * min_opt;
	}
	arq_acft1.close();

	ifstream arq_acft2((char*)str_y.c_str());
	for (n_acfts = 0; getline(arq_acft2, line); ++n_acfts) {
		if (line[1] == '#')
			break;
	}
	n_acfts--;
	arq_acft2.clear();
	arq_acft2.seekg(0, ios::beg);
	for (int i = 0; i<n_acfts; i++) {
		int cnf;
		string fltemp;
		arq_acft2 >> cnf >> fltemp;
		int flpos = flidtoflpos(fltemp);
		flts[flpos].result = -1;
		flts[flpos].org_result = -1;
		flts[flpos].cost = cancel_cost;
	}
	arq_acft2.close();


	for (int i = 0; i<results_namesga.size(); i++) results_namesga[i] = 0;
	ifstream arq_acft3((char*)str_g.c_str());
	for (n_acfts = 0; getline(arq_acft3, line); ++n_acfts) {
		if (line[1] == '#')
			break;
	}
	n_acfts--;
	arq_acft3.clear();
	arq_acft3.seekg(0, ios::beg);
	for (int i = 0; i<n_acfts; i++) {
		int cnf, result;
		string fltemp;
		arq_acft3 >> cnf >> fltemp >> result;
		int flt_pos = distance(namesga.begin(), find_if(namesga.begin(), namesga.end(), [fltemp](const string& a) { return a == fltemp; }));
		results_namesga[flt_pos] = result;
		struct_gas tempgas;
		tempgas.name = fltemp;
		tempgas.result = results_namesga[flt_pos];
		gas[cnf].push_back(tempgas);
	}
	arq_acft3.close();
	org_results_namesga = results_namesga;



	result_nos(); // atualiza nos com o resultado geral

	//show results
	cout << endl << endl << "Flights file Results" << endl;
	res_file << nome_instancia << endl;
	res_file << "objetivo: " << endl << result_1 << endl;
	res_file << "voo  config delay_opt cost" << endl;
	for (int i = 0; i<flts.size(); i++)
		if (find(lista_cfg.begin(), lista_cfg.end(), flts[i].cfg) != lista_cfg.end())
			if (cfgs[flts[i].cfg].model != "TranspCom")
				res_file << flts[i].flid << " " << cfgs[flts[i].cfg].model << " " << flts[i].result << endl;
	for (int i = 0; i<conf_namesga.size(); i++)
		for (int j = 0; j<lista_cfg.size(); j++)
			if (conf_namesga[i] == lista_cfg[j])
				res_file << namesga[i] << " " << conf_namesga[i] << " " << results_namesga[i] << endl;
	return 0;
}





string cplstr(string s, int l) {
	string st("");
	if (s.size()<l)
		for (int k = 0; k<(l - s.size()); k++)
			st += " ";
	return st + s;
}

void show_rotation(int result_1, string arqres) {
	//string arqres = "./Results/results_fifo_" +nome_instancia + "_" + to_string(noptions) + ".txt";
	ofstream res_file((char *)arqres.c_str());
	//show results
	vector <int> cancel_flts;
	vector < vector < int > >  flown_flts;

	int tl = 99;
	int lastcfg = 99;

	res_file << endl;

	//sort acfts by configuration
	vector < vector < int > > acftn;
	for (int j = 0; j<acfts.size(); j++)

		if (find(lista_cfg.begin(), lista_cfg.end(), acfts[j].cfg) != lista_cfg.end()) {
			vector < int > tempacftn;
			tempacftn.push_back(j);
			tempacftn.push_back(acfts[j].cfg);
			acftn.push_back(tempacftn);
		}
	sort(acftn.begin(), acftn.end(), [](const vector <int>&a, const vector <int>&b) {return a[1] < b[1]; });

	res_file << "Lista de Voos" << endl;
	for (int i = 0; i<lista_cfg.size(); i++) {
		int it_cfg = lista_cfg[i];
		//res_file << endl << "Modelo: " << it_cfg << " " << cfgs[it_cfg].model + " " + cfgs[it_cfg].cfg << endl;
		for (int j = 0; j<cfgs[it_cfg].fltids.size(); j++) {
			int sel = flidtoflpos(cfgs[it_cfg].fltids[j]);
			if (flts[sel].result >= 0)
				res_file << it_cfg << " " << cfgs[it_cfg].model + " " + cfgs[it_cfg].cfg << "  " << cplstr(flts[sel].flid, 6) << " " << cplstr(flts[sel].org, 6) << " " << cplstr(int_hora(flts[sel].dept + flts[sel].result*min_opt), 6) << " " << cplstr(flts[sel].dest, 6) << " " << cplstr(int_hora(flts[sel].arrv + flts[sel].result*min_opt), 6) << " " << cplstr(flts[sel].final_acft, 10) << " " << cplstr(flts[sel].org_acft, 10) << " " << cplstr(int_hora(flts[sel].result * min_opt), 6) << " " << cplstr(to_string(flts[sel].mant), 6) << endl;
			else
				res_file << it_cfg << " " << cfgs[it_cfg].model + " " + cfgs[it_cfg].cfg << "  " << cplstr(flts[sel].flid, 6) << " " << cplstr(flts[sel].org, 6) << " " << cplstr(int_hora(flts[sel].dept + flts[sel].result*min_opt), 6) << " " << cplstr(flts[sel].dest, 6) << " " << cplstr(int_hora(flts[sel].arrv), 6) << " " << cplstr(flts[sel].final_acft, 10) << " " << cplstr(flts[sel].org_acft, 10) << " " << cplstr(to_string(flts[sel].mant), 6) << "  Cancelado" << endl;
		}
	}
	
	for (int j = 0; j<acftn.size(); j++) {

		for (int i = 0; i<flts.size(); i++)
			if (find(lista_cfg.begin(), lista_cfg.end(), flts[i].cfg) != lista_cfg.end())
				if (flts[i].final_acft == acfts[acftn[j][0]].id)
					if (flts[i].result >= 0) {
						vector < int > tempflts;
						tempflts.push_back(i);
						tempflts.push_back(flts[i].dept + flts[i].result * min_opt);
						flown_flts.push_back(tempflts);
					}
					else
						cancel_flts.push_back(i);


		if (flown_flts.size()>0)
			sort(flown_flts.begin(), flown_flts.end(), [](const vector <int>&a, const vector <int>&b) { return a[1] < b[1]; });

		if (flown_flts.size()>0) {
			if ((flts[flown_flts[0][0]].cfg != lastcfg) && (flown_flts.size()>0)) {
				res_file << endl << "Trilhos" << endl;
				res_file << endl << "Modelo: " << flts[flown_flts[0][0]].cfg << " " << cfgs[flts[flown_flts[0][0]].cfg].model + " " + cfgs[flts[flown_flts[0][0]].cfg].cfg << endl;
				lastcfg = flts[flown_flts[0][0]].cfg;
				tl = 1;
			}
			int linha = 0;
			int TAT = 0;
			int sel, sel_ant;
			if (flown_flts.size()) {
				res_file << endl << "Trilho: " << tl << endl << "Aeronave: " << j << endl << cplstr("voo", 6) << cplstr("org", 7) << cplstr("dept", 7) << cplstr("dest", 7) << cplstr("arrv", 7) << cplstr("new_acft", 11) << cplstr("org_acft", 11) << cplstr("atraso", 7) << cplstr("maint", 7) << cplstr("TAT", 7) << cplstr("BT", 7) << endl;
				for (int i = 0; i<flown_flts.size(); i++) {
					sel = flown_flts[i][0];
					if (linha == 0)
						TAT = 0;
					else
						TAT = flts[sel].dept + flts[sel].result*min_opt - flts[sel_ant].arrv - flts[sel_ant].result * min_opt;
					res_file << cplstr(flts[sel].flid, 6) << " " << cplstr(flts[sel].org, 6) << " " << cplstr(int_hora(flts[sel].dept + flts[sel].result*min_opt), 6) << " " << cplstr(flts[sel].dest, 6) << " " << cplstr(int_hora(flts[sel].arrv + flts[sel].result*min_opt), 6) << " " << cplstr(flts[sel].final_acft, 10) << " " << cplstr(flts[sel].org_acft, 10) << " " << cplstr(int_hora(flts[sel].result * min_opt), 6) << " " << cplstr(to_string(flts[sel].mant), 6) << " " << cplstr(int_hora(TAT), 6) << " " << cplstr(int_hora(flts[sel].arrv- flts[sel].dept), 6) << endl;
					sel_ant = sel;
					linha++;
				}
				tl++;
			}

			if (flown_flts.size())
				flown_flts.clear();
		}
	}

	res_file << endl << "Voos Cancelados: ";

	res_file << endl << "Voos Cancelados: " << cancel_flts.size() << endl << "modelo voo org dept dest arrv new_acft org_acft time atraso" << endl;
	for (int i = 0; i<cancel_flts.size(); i++) {
		int sel = cancel_flts[i];
		res_file << cfgs[flts[sel].cfg].model + cfgs[flts[sel].cfg].cfg << " " << flts[sel].flid << " " << flts[sel].org << " " << int_hora(flts[sel].dept + flts[sel].result * min_opt) << " " << flts[sel].dest << " " << int_hora(flts[sel].arrv + flts[sel].result * min_opt) << " " << flts[sel].final_acft << " " << flts[sel].org_acft << " " << flts[sel].dept + flts[sel].result * min_opt << " " << int_hora(flts[sel].result * min_opt) << endl;
	}

	int sel;
	res_file << endl << endl << endl << "Rotação Aeroportuária" << endl;
	for (int i = 0; i<aerps.size(); i++) {
		vector < aepflts > vuelos;
		for (int j = 0; j<flts.size(); j++) {
			if (find(lista_cfg.begin(), lista_cfg.end(), flts[j].cfg) != lista_cfg.end()) {
				if ((flts[j].org == aerps[i].nome) && (flts[j].result >= 0)) {
					aepflts tempvuelo1;
					tempvuelo1.hora = flts[j].dept + flts[j].result*min_opt;
					tempvuelo1.tipo = 1;
					tempvuelo1.fltpos = j;
					vuelos.push_back(tempvuelo1);
				}
				if ((flts[j].dest == aerps[i].nome) && (flts[j].result >= 0)) {
					aepflts tempvuelo2;
					tempvuelo2.hora = flts[j].arrv + flts[j].result*min_opt;
					tempvuelo2.tipo = 0;
					tempvuelo2.fltpos = j;
					vuelos.push_back(tempvuelo2);
				}
			}
		}
		sort(vuelos.begin(), vuelos.end(), comp_timee2);
		res_file << endl << endl << endl << endl << "==============================================================================    " << aerps[i].nome << "   ========================================================================================== " << endl << endl;
		for (int j = 0; j<vuelos.size(); j++) {
			sel = vuelos[j].fltpos;
			string arc = flts[sel].flid + " " + cplstr(flts[sel].org, 8) + " " + cplstr(int_hora(flts[sel].dept + flts[sel].result*min_opt), 6) + " " + cplstr(flts[sel].dest, 6) + " " + cplstr(int_hora(flts[sel].arrv + flts[sel].result*min_opt), 6) + " " + cplstr(flts[sel].final_acft, 10) + " " + cplstr(flts[sel].org_acft, 10) + " " + cplstr(int_hora(flts[sel].result * min_opt), 6) + " " + cplstr(to_string(flts[sel].mant), 6) + " ";
			string strhora = int_hora(vuelos[j].hora);
			if (vuelos[j].tipo == 0)
				res_file << arc << esps(80 - arc.length()) << strhora << endl;
			else
				res_file << esps(80) << strhora << esps(7) << arc << endl;

		}
		vuelos.clear();
	}

	res_file.close();
}


void show_rotation_heuristic(string arqres) {
	int sel;
	ofstream res_file((char *)arqres.c_str());
	int cont = 1, hant, TAT, cont_trilho;
	string arc_saida;
	for (int i = 0; i<trilhos.size(); i++) {

		if (i>0)
			if (trilhos[i].cnf != trilhos[i - 1].cnf) {
				cont = 1;
				res_file << endl;
			}


		int cfg = trilhos[i].cnf;
		//res_file << endl << "Modelo: " << cfg  << " " << cfgs[cfg].model + " " + cfgs[cfg].cfg << endl;
		res_file << endl << "Aeronave: " << trilhos[i].acft << "    " << "Modelo: " << cfg << " " << cfgs[cfg].model + " " + cfgs[cfg].cfg << " Manutencao: " << trilhos[i].mant << " | " << "Trilho: " << i << endl;
		res_file << cplstr("voo", 8) << cplstr("org", 9) << cplstr("dept", 7) << cplstr("dest", 7) << cplstr("arrv", 7) << cplstr("new_acft", 11) << cplstr("org_acft", 11) << cplstr("atraso", 7) << cplstr("maint", 7) << cplstr("TAT", 7) << endl;

		hant = 0;
		cont_trilho = 0;
		for (int j = 0; j<trilhos[i].trilhos.size(); j++) {
			arc_saida = trilhos[i].trilhos[j];
			/*if (arc_saida.at(0)!='g') {
			int sel = arctoflpos(arc_saida);
			if (j=0)
			TAT = 0;
			else
			TAT = flts[sel].dept+flts[sel].result*min_opt - hant;

			res_file  << cplstr(flts[sel].flid,6) << " " << cplstr(flts[sel].org,6) << " " << cplstr(int_hora(flts[sel].dept+flts[sel].result*min_opt),6) << " " << cplstr(flts[sel].dest,6) << " " << cplstr(int_hora(flts[sel].arrv+flts[sel].result*min_opt),6)  << " " << cplstr(flts[sel].final_acft,10) << " " << cplstr(flts[sel].org_acft,10) << " " << cplstr(int_hora(flts[sel].result*min_opt),6) << " " << cplstr(to_string(flts[sel].mant),6) << " " << cplstr(int_hora(TAT),6) << endl;
			hant = flts[sel].arrv+flts[sel].result*min_opt;
			// res_file << arc_saida << endl;
			}*/
			sel = arctoflpos(arc_saida);

			if (arc_saida.at(0) != 'g') {

				if (cont_trilho == 0)
					TAT = 0;
				else
					TAT = flts[sel].dept + flts[sel].result*min_opt - hant;
				//res_file << arc_saida << " " << arctoflpos(arc_saida) << endl;
				//res_file  << cplstr(flts[sel].flid,6) << " " << cplstr(flts[sel].org,6) << " " << cplstr(int_hora(flts[sel].dept+flts[sel].result*min_opt),6) << " " << cplstr(flts[sel].dest,6) << " " << cplstr(int_hora(flts[sel].arrv+flts[sel].result*min_opt),6)  << " " << cplstr(flts[sel].final_acft,10) << " " << cplstr(flts[sel].org_acft,10) << " " << cplstr(int_hora(flts[sel].result*30),6) << " " << cplstr(to_string(flts[sel].mant),6) << endl;
				res_file << arc_saida << " " << cplstr(flts[sel].org, 8) << " " << cplstr(int_hora(flts[sel].dept + flts[sel].result*min_opt), 6) << " " << cplstr(flts[sel].dest, 6) << " " << cplstr(int_hora(flts[sel].arrv + flts[sel].result*min_opt), 6) << " " << cplstr(flts[sel].final_acft, 10) << " " << cplstr(flts[sel].org_acft, 10) << " " << cplstr(int_hora(flts[sel].result * min_opt), 6) << " " << cplstr(to_string(flts[sel].mant), 6) << " " << cplstr(int_hora(TAT), 6) << endl;
				hant = flts[sel].arrv + flts[sel].result*min_opt;
				cont_trilho++;
			}
		}
		cont++;
	}




	res_file << endl << endl << endl << "Rotação Aeroportuária" << endl;
	for (int i = 0; i<aerps.size(); i++) {
		vector < aepflts > vuelos;
		for (int j = 0; j<flts.size(); j++) {
			if (find(lista_cfg.begin(), lista_cfg.end(), flts[j].cfg) != lista_cfg.end()) {
				if ((flts[j].org == aerps[i].nome) && (flts[j].result >= 0)) {
					aepflts tempvuelo1;
					tempvuelo1.hora = flts[j].dept + flts[j].result*min_opt;
					tempvuelo1.tipo = 1;
					tempvuelo1.fltpos = j;
					vuelos.push_back(tempvuelo1);
				}
				if ((flts[j].dest == aerps[i].nome) && (flts[j].result >= 0)) {
					aepflts tempvuelo2;
					tempvuelo2.hora = flts[j].arrv + flts[j].result*min_opt;
					tempvuelo2.tipo = 0;
					tempvuelo2.fltpos = j;
					vuelos.push_back(tempvuelo2);
				}
			}
		}
		sort(vuelos.begin(), vuelos.end(), comp_timee2);
		res_file << endl << endl << endl << endl << "==============================================================================    " << aerps[i].nome << "   ========================================================================================== " << endl << endl;
		for (int j = 0; j<vuelos.size(); j++) {
			sel = vuelos[j].fltpos;
			string arc = flts[sel].flid + " " + cplstr(flts[sel].org, 8) + " " + cplstr(int_hora(flts[sel].dept + flts[sel].result*min_opt), 6) + " " + cplstr(flts[sel].dest, 6) + " " + cplstr(int_hora(flts[sel].arrv + flts[sel].result*min_opt), 6) + " " + cplstr(flts[sel].final_acft, 10) + " " + cplstr(flts[sel].org_acft, 10) + " " + cplstr(int_hora(flts[sel].result * min_opt), 6) + " " + cplstr(to_string(flts[sel].mant), 6) + " ";
			string strhora = int_hora(vuelos[j].hora);
			if (vuelos[j].tipo == 0)
				res_file << arc << esps(80 - arc.length()) << strhora << endl;
			else
				res_file << esps(80) << strhora << esps(7) << arc << endl;

		}
		vuelos.clear();
	}

	res_file.close();

}

void monta_R(string arqres) {
	ofstream res_file((char *)arqres.c_str());
	res_file << "beg end" << endl << recovery_time.beginning << " " << (recovery_time.d2 - min_date) * 1440 + recovery_time.t2 << endl;
	res_file << "model,acft_org,hbt,dept,arrv,od,voo,result,dept_final,arrv_final,acft_novo,mant,acft_bd,cnf,alt_flt" << endl;
	for (int j = 0; j<lista_cfg.size(); j++)
		for (int i = 0; i<flts.size(); i++)
			if (lista_cfg[j] == flts[i].cfg)
				if ((flts[i].result >= 0) && (flts[i].final_acft != ""))
					res_file << cfgs[flts[i].cfg].model + "_" + to_string(cfgs[flts[i].cfg].ass) + "ASS" << "," << flts[i].org_acft << "," << flts[i].arrv - flts[i].dept << "," << flts[i].dept << "," << flts[i].arrv << "," << flts[i].org + "-" + flts[i].dest << "," << flts[i].flid << "," << flts[i].result << "," << flts[i].dept + flts[i].result*min_opt << "," << flts[i].arrv + flts[i].result*min_opt << "," << flts[i].final_acft << "," << mant_flid(flts[i].flid) << "," << flid_alt_acft(flts[i].flid) << "," << flts[i].cfg << "," << flts[i].atr << endl;
				else
					res_file << cfgs[flts[i].cfg].model + "_" + to_string(cfgs[flts[i].cfg].ass) + "ASS" << "," << flts[i].org_acft << "," << flts[i].arrv - flts[i].dept << "," << flts[i].dept << "," << flts[i].arrv << "," << flts[i].org + "-" + flts[i].dest << "," << flts[i].flid << "," << flts[i].result << "," << flts[i].dept << "," << flts[i].arrv << "," << flts[i].org_acft << "," << mant_flid(flts[i].flid) << "," << flid_alt_acft(flts[i].flid) << "," << flts[i].cfg << "," << flts[i].atr << endl;

	res_file.close();
}


int le_r1_fifo() {
	ofstream cout((char*)nome_output.c_str(), std::fstream::app);
	string mtes = "./Results/" + nome_instancia + "/tst_ler.txt";
	//ofstream res_file((char *)mtes.c_str());

	for (int i = 0; i<lps.size(); i++) {
		string nomelp = lps[i];
		string nomeresult = lps[i].substr(0, lps[i].length() - 3) + ".sol";

		//res_file <<  nomelp << " " << nomeresult << endl;
		string line, line2;
		int result;


		//string str = nomeresult;

		// string str2 =  "./Results/" + nome_instancia + "/res_atrib_" + nomelp + ".txt";

		string str2 = lps[i].substr(0, lps[i].length() - 3) + "atr.sol";


		string comadx = "gawk '{ if ($2!=0) print $0 }' " + nomeresult + " > " + str2;
		int roda = system((char*)comadx.c_str());


		ifstream arq_acft((char*)str2.c_str());
		int cont_line = 0;
		while (getline(arq_acft, line)) {
			//res_file << line << endl;
			vector <string> sep = split(line, ' ');
			//res_file << sep[0] << " " << sep[1] << endl;
			if (sep[0][0] == '#')
				result_1 = stoi(sep[sep.size() - 1]);
			else if (sep[0][0] == 'x') {
				//res_file << sep[0] << " " << sep[1] << endl;
				if (sep[sep.size() - 1][0] == '1') {
					vector <string> sep2 = split(sep[0], '_');
					string fltr = sep2[2];
					int fltr_dl = stoi(sep2[3]);
					int flt_pos = distance(flts.begin(), find_if(flts.begin(), flts.end(), [fltr](const flight_struct& a) { return a.flid == fltr; }));
					//flts[flt_pos].result = fltr_dl;
					//res_file << fltr << " " << fltr_dl << endl;
					int acf_pos = stoi(sep2[4]);
					flts[flt_pos].final_acft = acfts[acf_pos].id;
					//res_file << fltr << " " << acfts[acf_pos].id << endl;
				}
			}
			else {
				if (sep[sep.size() - 1][0] == '1') {
					string flt_temp = sep[0].substr(0, sep[0].find_last_of("_") - 1);
					int flt_pos = distance(namesga.begin(), find_if(namesga.begin(), namesga.end(), [flt_temp](const string& a) { return a == flt_temp; }));
					acfts_namesga[flt_pos] = stoi(sep[0].substr(sep[0].find_last_of("_") + 1, sep[0].npos));
				}
			}
			cont_line++;
		}
		if (cont_line == 0)
			result_1 = -1;
		arq_acft.close();
	}
	return result_1;
}

vector < string > outs(int cnf, int aerp, int pos) {
	vector < string > tempout;
	//cout << cnf << " " << aerp << " " << pos << nos[cnf][aerp][pos].outfltarc.size() << endl;
	for (int i = 0; i<nos[cnf][aerp][pos].outfltarc.size(); i++) {
		string arc = nos[cnf][aerp][pos].outfltarc[i];
		if ((!flts[arctoflpos(arc)].fifo) && (arc_result(arc)))
			tempout.push_back(arc);
	}
	for (int i = 0; i<nos[cnf][aerp][pos].outgrdarc.size(); i++) {
		if (result_ga(nos[cnf][aerp][pos].outgrdarc[i])>0)
			tempout.push_back(nos[cnf][aerp][pos].outgrdarc[i]);

	}
	return tempout;
}

string key2_arc(string arc_saida) {
	string tkey2;
	if (arc_saida.at(0) != 'g') {
		int res_arc = stoi(arc_saida.substr(arc_saida.find("_") + 1, arc_saida.npos));
		int pos = arctoflpos(arc_saida);

		int tt;
		if (flts[pos].thr_pre)
			tt = cfgs[flts[pos].cfg].tt;
		else
			tt = cfgs[flts[pos].cfg].tat;

		tkey2 = flts[pos].dest + to_string(flts[pos].arrv + min_opt*res_arc + tt);
		flts[arctoflpos(arc_saida)].fifo = true;
		flts[arctoflpos(arc_saida)].result = -1;
	}
	else {
		//ga_215_12_MRS1905_MRS1910
		int np = arc_saida.size() - 1;
		while (arc_saida.at(np) != '_') np--;

		auto found = arc_saida.find("OUT");
		if (found == std::string::npos)
			tkey2 = arc_saida.substr(np + 1, arc_saida.npos);
		else
			tkey2 = "OUT_" + arc_saida.substr(np + 1, arc_saida.npos);



		//reduce 1 in ground arc
		auto it = find(namesga.begin(), namesga.end(), arc_saida);
		if (it != namesga.end()) {
			int pos = distance(namesga.begin(), it);
			results_namesga[pos]--;
		}
	}
	return tkey2;
}

int acfts_pos(string id) {
	return distance(acfts.begin(), find_if(acfts.begin(), acfts.end(), [id](const aircraft_struct& a) { return a.id == id; }));
}

struct listin {
	int cnf, aerp_pos;
	string init_aerp;
	string id_acft;
	string flid_mant = "";
	bool mant = false;
	//  vector < int > pot_afts;
};

vector <string> inters(vector <string> v1, vector <string> v2) {
	vector <string> temp;
	for (int i = 0; i<v2.size(); i++)
		for (int j = 0; j<v1.size(); j++)
			if (v2[i] == v1[j])
				temp.push_back(v2[i]);
	return temp;
}

string escolhe_arco_pref2(vector < string > toutarcs) {

	string arc_saida;

	vector < string > arc_voos;
	for (int i = 0; i<toutarcs.size(); i++) {
		arc_saida = toutarcs[i];
		if (arc_saida.at(0) != 'g')
			if (!mant_arc(arc_saida))
				arc_voos.push_back(arc_saida);
	}

	vector < string > arc_g;
	for (int i = 0; i<toutarcs.size(); i++) {
		arc_saida = toutarcs[i];
		if (arc_saida.at(0) == 'g')
			arc_g.push_back(arc_saida);
	}

	vector < string > arc_mant;
	for (int i = 0; i<toutarcs.size(); i++) {
		arc_saida = toutarcs[i];
		if (arc_saida.at(0) != 'g')
			if (mant_arc(arc_saida))
				arc_mant.push_back(arc_saida);
	}

	vector < string > juncao = arc_voos;
	juncao.insert(juncao.begin(), arc_g.begin(), arc_g.end());
	if (juncao.size() == 0)
		juncao.insert(juncao.begin(), arc_mant.begin(), arc_mant.end());

	//cout<< "juncao: ";
	//for (int i=0;i<juncao.size();i++) cout << juncao

	std::random_device rd{};
	std::mt19937 engine{ rd() };
	std::uniform_real_distribution<double> dist{ 0.0, 1.0 };
	double x = dist(engine);
	int chosen_arc = round(x * (juncao.size() - 1));
	string arc_saida2 = juncao[chosen_arc];
	return arc_saida2;
}


string escolhe_arco_pref3(vector < string > toutarcs) {

	vector < string > juncao = toutarcs;

	std::random_device rd{};
	std::mt19937 engine{ rd() };
	std::uniform_real_distribution<double> dist{ 0.0, 1.0 };
	double x = dist(engine);
	int chosen_arc = round(x * (juncao.size() - 1));
	string arc_saida2 = juncao[chosen_arc];
	return arc_saida2;
}


int aerppos(string nome, int cnf) {
	int i = 0;
	int res;
	//cout << " doe " << nome << endl;
	for (i = 0; i<nos[cnf].size(); i++)

		if ((nos[cnf][i].size()>0) && (nos[cnf][i][0].local == nome)) {
			res = i;
			break;
		}
	//cout << res << endl;
	return res;
}

trilho_struct busca_trilho(string id_acft, int cnf, string init_aerp, string flid_mant) {

	vector < string > toutarcs;
	int aerp = aerppos(init_aerp, cnf);
	string arc_saida;
	toutarcs = outs(cnf, aerp, 0);

	string mant_arco = flid_mant;
	if (flid_mant.size()>0)
		mant_arco += "_0";

	trilho_struct temp_trilho;
	temp_trilho.cnf = cnf;
	temp_trilho.init_aerp = init_aerp;
	temp_trilho.acft = id_acft;


	int aerp2;

	bool manto = false;


	//   cout << "2.1" << endl;

	while (toutarcs.size()>0) {



		bool saida = true;
		//  cout << "Acft: "  << id_acft << "  arcos: ";
		//     for (int i=0;i<toutarcs.size();i++ ) cout << toutarcs[i] << " ";
		//   cout << "arco de manutencao: " << mant_arco << endl;

		auto it = find(toutarcs.begin(), toutarcs.end(), mant_arco);

		if ((flid_mant.size()>0) && (it != toutarcs.end())) {
			//      cout << "mant" << endl;
			arc_saida = mant_arco;
			manto = true;
			temp_trilho.mant = true;
		}
		else {
			arc_saida = escolhe_arco_pref2(toutarcs);
			//     cout << "not mant" << endl;
		}
		temp_trilho.trilhos.push_back(arc_saida);
		//    cout << arc_saida << " 2.3" << endl;
		vector < string > keys = arctokeys(arc_saida);
		string key2 = keys[2];
		string aep2 = keys[3];
		aerp2 = aerppos(aep2, cnf);
		//  cout << "2.4" << endl;
		if (arc_saida.at(0) != 'g') {
			int posarc = arctoflpos(arc_saida);
			flts[posarc].fifo = true;
			flts[posarc].final_acft = id_acft;
		}
		else {
			//reduce 1 in ground arc
			auto it = find(namesga.begin(), namesga.end(), arc_saida);
			if (it != namesga.end()) {
				int pos = distance(namesga.begin(), it);
				results_namesga[pos]--;
			}
		}
		//  cout << "2.5" << endl;

		vector < activity_struct > nodes = nos[cnf][aerp2];
		int node_pos = distance(nodes.begin(), find_if(nodes.begin(), nodes.end(), [key2](const activity_struct& a) { return a.key == key2; }));
		toutarcs = outs(cnf, aerp2, node_pos);
		//  cout << "2.6" << endl;

		// cout << endl;
		//   cout << arc_saida << " " << toutarcs.size() << endl;
	}
	return temp_trilho;
}

bool contem_arc(trilho_struct temp_trilho, string flid_mant) {
	string arc_saida;
	int mant = 0;
	bool arcomant = false;

	for (int i = 0; i < temp_trilho.trilhos.size(); i++) {
		arc_saida = temp_trilho.trilhos[i];
		if (arc_saida.at(0) != 'g') {
			if (mant_arc(arc_saida))
				mant++;
			if (arc_saida == (flid_mant + "_0"))
				arcomant = true;
		}
	}
	if ((arcomant) && (mant == 1))
		return true;
	else
		return false;
}

void monta_fifo_heuristic() {

	string arqres = "./Results/" + nome_instancia + "/avaliacao_" + nome_instancia + "_" + to_string(noptions) + ".txt";
	ofstream res_file((char *)arqres.c_str());
	string arc_saida;
	vector < listin > list_inacft;
	vector < flight_struct > flts_org;
	vector < int > results_namesga_org;
//	bool saida;
	cont_mant = 0;
	cont_mant_exec = 0;
	for (int p = 0; p < acfts.size(); p++) {
		listin temp_lis;
		temp_lis.id_acft = acfts[p].id;
		temp_lis.init_aerp = acfts[p].airp;
		temp_lis.cnf = acfts[p].cfg;

		if (acfts[p].mant != "NULL") {
			temp_lis.mant = true;
			for (int i = 0; i<flts.size(); i++)
				if ((flts[i].mant) && (flts[i].org_acft == acfts[p].id)) {
					temp_lis.flid_mant = flts[i].flid;
					break;
				}
		}
		if (find(lista_cfg.begin(), lista_cfg.end(), temp_lis.cnf) != lista_cfg.end())
		{
			list_inacft.push_back(temp_lis);
			//     cout << "trilho: " << temp_lis.id_acft << endl;

		}
	}



	//define trilhos com os arcos manutencao
	for (int i = 0; i<list_inacft.size(); i++) {
		trilho_struct temp_trilho;
		//  cout << list_inacft[i].id_acft << " " << list_inacft[i].cnf << " " << list_inacft[i].init_aerp << " " << list_inacft[i].flid_mant << endl;
		if (list_inacft[i].mant) {

			cont_mant++;
			//  cout << "1" << endl;
			flts_org = flts;
			results_namesga_org = results_namesga;
			int niter = 0;
			do {
				flts = flts_org;
				results_namesga = results_namesga_org;
				temp_trilho = busca_trilho(list_inacft[i].id_acft, list_inacft[i].cnf, list_inacft[i].init_aerp, list_inacft[i].flid_mant);
				niter++;
				if (contem_arc(temp_trilho, list_inacft[i].flid_mant)) {
					cont_mant_exec++;
					break;
				}
				//cout << list_inacft[i].id_acft << " " << list_inacft[i].flid_mant << " " << niter << endl;
			} while (niter<50);
			//  cout << "2" << endl;
			//flts = flts_org;
			//results_namesga = results_namesga_org;
			trilhos.push_back(temp_trilho);
		}
	}


	//define trilhos com os arcos nao manutencao
	for (int i = 0; i<list_inacft.size(); i++) {
		trilho_struct temp_trilho;
		//  cout << list_inacft[i].id_acft << " " << list_inacft[i].cnf << " " << list_inacft[i].init_aerp << " " << list_inacft[i].flid_mant << endl;
		if (!list_inacft[i].mant) {
			//  cout << "1" << endl;
			//flts_org = flts;
			//results_namesga_org = results_namesga;
			temp_trilho = busca_trilho(list_inacft[i].id_acft, list_inacft[i].cnf, list_inacft[i].init_aerp, list_inacft[i].flid_mant);
			//  cout << "2" << endl;
			//flts = flts_org;
			//results_namesga = results_namesga_org;
			trilhos.push_back(temp_trilho);
		}
	}
}


#endif