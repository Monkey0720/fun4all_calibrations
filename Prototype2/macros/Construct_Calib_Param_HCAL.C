// $$Id: $$

#include <string>
#include <fstream>      // std::fstream
using namespace std;

void
Construct_Calib_Param_HCAL(
	const char* hcalin_lg_MIP_peak ="cosmic_hcalin_lg.txt",
	const char* hcalin_hg_MIP_peak ="cosmic_hcalin_hg.txt",
	const char* hcalout_lg_MIP_peak ="cosmic_hcalout_lg.txt",
	const char* hcalout_hg_MIP_peak ="cosmic_hcalout_hg.txt" )
{

  cout << "Construct_Calib_Param() - Entry" << endl;
  gSystem->Load("libg4detectors.so");
  char *det_name[] = {"HCALIN","HCALOUT"};
  char *gain_name[] = {"LG","HG"};

  double expected_hcalin[] = {8.6, 8.1, 7.6, 7.2}; //MeV
  double expected_hcalout[] = {6.7, 6.8, 7.0, 7.2}; //MeV
  TString name; 
  fstream f; 

  for(int idet=0; idet<2; idet++)
  {
   for(int igain=0; igain<2; igain++)
   {
    name = det_name[idet];
    name += "_";
    name += gain_name[igain];
    cout << name << endl;
    PHParameters * param = new PHParameters(name.Data());
    param->set_double_param("calib_const_scale", -1.);
    param->set_int_param("use_chan_calibration", 1);
    param->set_string_param("description", "Calibration based on MIP scans");

    if(idet==0&&igain==0) f.open(hcalin_lg_MIP_peak, ios_base::in);
    if(idet==0&&igain==1) f.open(hcalin_hg_MIP_peak, ios_base::in);
    if(idet==1&&igain==0) f.open(hcalout_lg_MIP_peak, ios_base::in);
    if(idet==1&&igain==1) f.open(hcalout_hg_MIP_peak, ios_base::in);
    assert(f.is_open());
    int count = 0;
    while (!f.eof())
    {
      string line;
      getline(f, line);

      if (line.length() == 0)
        continue;

      int col = -1, row = -1;
      double mip = -1;

      stringstream sline(line);

      sline >> col >> row >> mip;

      cout << "col = " << col << ", row = " << row << ", mip = " << mip << endl;

      assert(col >= 0);
      assert(row >= 0);
      assert(mip > 0);

      string calib_const_name(Form("calib_const_column%d_row%d", col, row));

      double expected_mip = 0; //expected MIP in GeV
      if(idet==0) expected_mip = expected_hcalin[row];
      else if(idet==1) expected_mip = expected_hcalout[row];
      //expected_mip *= 0.001; //MeV to GeV
      param->set_double_param(calib_const_name, expected_mip / mip);
      count++;

    }

  // storage
  param->print();

//  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
//  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  //param->WriteToFile("xml", "../Calibration/");
  param->WriteToFile("xml", "./");
  f.close();
  }
 }

}

void
Construct_Calib_Param_Constatnt_Test()
{
  cout << "Construct_Calib_Param() - Entry" << endl;

  gSystem->Load("libg4detectors.so");

  PHParameters * param = new PHParameters("CEMC");

  param->set_string_param("description",
      "Test file with same constant for all towers");

  // additional scale for the calibration constant
  // negative pulse -> positive with -1
  param->set_double_param("calib_const_scale", -1);

  // use channel by channel stuff
  param->set_int_param("use_chan_calibration", 1);

//  feed the numbers
  for (int col = 0; col < 8; col++)
    for (int row = 0; row < 8; row++)
      {

        string calib_const_name(Form("calib_const_column%d_row%d", col, row));

        param->set_double_param(calib_const_name, 1);

      }

  // storage
  param->print();

//  param->WriteToFile("root", "./test_geom/");

  // this to solve an annoying problem for ROOT/XML regarding precision needed for Geant4
//  TBufferXML::SetFloatFormat("%.18e"); // match precision for double
  param->WriteToFile("xml", "../Calibration/");
//  param->WriteToFile("xml", "./");

}

