#include "DataCreator.h"

void DataCreator::Create(const std::string & configFile)
{
 // do somethin
  std::ofstream oFile ("Student.tab", std::ios::out | std::ios::binary);
  const char * data = "02389389392Greig               Hazell              M0101SR0238938938213949493484Andrew              Bender              M2001JR23879093894Emma                Turetsky            F2010SR";
  oFile.write(data, 183);
  oFile.close();
}

void DataCreator::CreateDB(const std::string & configFile, bool makeHuman)
{
  std::ifstream dataFormat(configFile.c_str());
  std::string tabName;
  while(!std::getline(dataFormat,tabName).eof())
  {
    std::string tableName = "Data/" + tabName;
    std::ofstream oFile (tableName.c_str(), std::ios::out | std::ios::binary);
    std::string numFields;
    std::string numRecords;
    std::string numBytes;
    std::getline(dataFormat,numFields,'|');
    std::getline(dataFormat,numRecords,'|');
    std::getline(dataFormat,numBytes);
    int nFields = atoi(numFields.c_str());
    int nRecords = atoi(numRecords.c_str());
    int nBytes = atoi(numBytes.c_str());
    std::vector<std::pair<bool,int> > * types = new std::vector<std::pair<bool, int> >();
    char * record_buf = new char[nBytes*nRecords];
      for(int i = 0; i < nFields; i++)
      {
        std::string fType;
        std::getline(dataFormat,fType,'|');
        if(fType.compare("int") == 0)
        {   
	  std::pair<bool,int> ty(true,4);
	  //ty = std::make_pair(true,4);
	  types->push_back(ty);
          std::string iType;
          std::getline(dataFormat,iType,'|');
          if(iType.compare("incr") == 0)
          {
            std::string start;
	    std::getline(dataFormat,start);
	    int s = atoi(start.c_str());
	    for (int k = 0; k < nRecords; k++)
	    {
	      int field = k;
	      memcpy(record_buf + (nBytes*k)+s,&field,sizeof(field));
	    }
          }
          else if(iType.compare("randIncr") == 0)
          {
            std::string start;
            std::getline(dataFormat,start);
            int s = atoi(start.c_str());
            int field = rand() % 50;
            memcpy(record_buf +s,&field,sizeof(field));
            for (int k = 1; k < nRecords; k++)
            {
              int inc = (rand() % 100) + 1;
              field += inc;
              memcpy(record_buf+(nBytes*k)+s,&field,sizeof(field));
            }
          }
          else if(iType.compare("range") == 0)
          {
            std::string lower;
            std::string upper;
            std::string nLower;
            std::string nUpper;
            std::getline(dataFormat,lower,'|');
            std::getline(dataFormat,upper,'|');
            std::getline(dataFormat,nLower,'|');
            std::getline(dataFormat,nUpper,'|');
	    std::string start;
	    std::getline(dataFormat,start);
            int s = atoi(start.c_str());
            int lB = atoi(lower.c_str());
            int uB = atoi(upper.c_str());
            int nLB = atoi(nLower.c_str());
            int nUB = atoi(nUpper.c_str());
            int diff = uB - lB + 1;
	    for (int k = 0; k < nRecords; k++)
	    {
	      int field = rand() % diff + lB;
	      if ((field < nUB) && (field > nLB))
	      {
	        field = rand() % (1+lB-nLB) + lB;
	      }
              memcpy(record_buf + (nBytes*k)+s, &field, sizeof(field));
	    }
          }
          else if(iType.compare("oddRange") == 0)
          {
            std::string lower;
            std::string upper;
            std::getline(dataFormat,lower,'|');
            std::getline(dataFormat,upper,'|');
            std::string start;
            std::getline(dataFormat,start);
            int s = atoi(start.c_str());
            int lB = atoi(lower.c_str());
            int uB = atoi(upper.c_str());
            int diff = uB - lB + 1;
            for (int k = 0; k < nRecords; k++)
            {
              int field = (rand() % diff) + lB;
              if ((field % 2) == 0)
              {
                if (field == uB)
                {
                 field--;
                }
                else
                {
                 field++;
                }
              }
              memcpy(record_buf + (nBytes*k)+s, &field, sizeof(field));
            }
          }
          else if(iType.compare("evenRange") == 0)
          {
            std::string lower;
            std::string upper;
            std::getline(dataFormat,lower,'|');
            std::getline(dataFormat,upper,'|');
            std::string start;
            std::getline(dataFormat,start);
            int s = atoi(start.c_str());
            int lB = atoi(lower.c_str());
            int uB = atoi(upper.c_str());
            int diff = uB - lB + 1;
            for (int k = 0; k < nRecords; k++)
            {
              int field = (rand() % diff) + lB;
              if ((field % 2) != 0)
              {
                if (field == uB)
                {
                 field--;
                }
                else
                {
                 field++;
                }
              }
              memcpy(record_buf + (nBytes*k)+s, &field, sizeof(field));
            }
          }
          else
          {
	   std::string start;
	   std::getline(dataFormat,start);
	   int s = atoi(start.c_str());
	   for (int k = 0; k <nRecords; k++)
	   {
	     int field = rand();
             memcpy(record_buf + (nBytes*k)+s, &field, sizeof(field));
	   }
          }
        }
        else if (fType.compare("fk") == 0)
        {
	    std::pair<bool,int> ty(true,4);
	    //ty = std::make_pair(true,4);
            types->push_back(ty);

            std::string lower;
            std::string upper;
            std::string nLower;
            std::string nUpper;
            std::getline(dataFormat,lower,'|');
            std::getline(dataFormat,upper,'|');
            std::getline(dataFormat,nLower,'|');
            std::getline(dataFormat,nUpper,'|');
            std::string start;
            std::getline(dataFormat,start);
            int s = atoi(start.c_str());
            int lB = atoi(lower.c_str());
            int uB = atoi(upper.c_str());
            int nLB = atoi(nLower.c_str());
            int nUB = atoi(nUpper.c_str());
            int diff = uB - lB + 1;
           std::vector<int> * fKey = new std::vector<int>(); 
           for (int k = 0; k < nRecords; k++)
            {
              int field = rand() % diff + lB;
              if ((field < nUB) && (field > nLB))
              {
                field = rand() % (1+lB-nLB) + lB;
              }
              fKey->push_back(field);
            }
            std::vector<int>::iterator it;
            sort (fKey->begin(), fKey->end());
	    std::cerr << fKey->size() << "\n";
            for (int l = 0; l < nRecords; l++)
            { 
              int a = fKey->at(l);
              memcpy(record_buf + (nBytes*l)+s, &a, sizeof(a));
            }
        }
        else
        {
          std::string numChar;
          std::getline(dataFormat,numChar,'|');
          int nChar = atoi(numChar.c_str());
          char * field = new char[nChar+1];
	  std::string start;
	  std::getline(dataFormat,start);
          int s = atoi(start.c_str());
          for (int k = 0; k < nRecords; k++)
	  {
	    for (int z = 0; z < nChar; z++)
            {
              int n = rand() % 26;
              char c = (char)(n+65);
              field[z] = c;
            }
            memcpy(record_buf + (nBytes*k)+s, field, nChar);
	    field[nChar] = 0;
	    std::pair<bool,int> ty(false,nChar);
	    //ty = std::make_pair(false,nChar);
            types->push_back(ty);
	  }
        }
      }
      oFile.write(record_buf, nBytes*nRecords);
    oFile.close();
    if(makeHuman)
    {
      int offset = 0;
      std::string hName = tableName + ".out";
      std::ofstream hFile(hName.c_str());
      for (int b = 0; b < nRecords; b++)
      {
        for (int a = 0; a < nFields; a++)
	{ 
	  std::pair<bool,int> ty = types->at(a);
	  if (ty.first)
	  {
	    int number;
	    memcpy(&number, record_buf + offset, sizeof(int));
	    hFile << number << "|";
	    offset += 4;
	  }
	  else
	  {
	    int nChar = ty.second;
	    char * str = new char[nChar+1];
	    memset(str,0,nChar+1);
	    memcpy(str, record_buf+offset, nChar);
	    offset += nChar;
	    hFile << str << "|";
	  }
	}
	hFile << "\n";
      }
      hFile.close();
    }
    delete record_buf;
  }
  dataFormat.close();
}
