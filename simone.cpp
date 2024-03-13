//The structure for gamma matrix
#include <complex>

using namespace std;

struct dirac_matr
{
  int pos[4];
  complex<double> entr[4];
  
  constexpr dirac_matr operator*(const dirac_matr& oth) const
  {
    dirac_matr out{};
    
    //This is the line on the first matrix
    for(int ig1=0;ig1<4;ig1++)
      {
	//This is the line to be taken on the second matrix
	const int ig2=this->pos[ig1];
	
	//For each line, the column of the output matrix which is
	//different from 0 is the column of the second matrix different
	//from 0 on the line with index equal to the column of the first
	//matrix which is different from 0 (that is, ig2)
	out.pos[ig1]=oth.pos[ig2];
	
	//The entries of the output is, on each line, the complex
	//product of the entries of the first matrix on that line, for
	//the entries of the second matrix on the line with the index
	//equal to the column of the first matrix which is different
	//from 0 (which again is ig2)
	// out.entr[ig1]=this->entr[ig1]*oth.entr[ig2];// const auto& g1=this->entr[ig1],g2=oth.entr[ig2];
	const auto& g1=this->entr[ig1],g2=oth.entr[ig2];
	const auto& r1=g1.real(),r2=g2.real();
	const auto& i1=g1.imag(),i2=g2.imag();
	const double r=r1*r2-i1*i2;
	const double i=r1*i2+r2*i1;
	out.entr[ig1]={r,i};
      }
    
    return out;
  }
  
  constexpr dirac_matr dag() const
  {
    dirac_matr out{};
    
    for(int ig1=0;ig1<4;ig1++)
      {
	const int ig2=pos[ig1];
	out.pos[ig2]=ig1;
	out.entr[ig2]=conj(entr[ig1]);
      }
    
    return out;
  }
  
  constexpr complex<double> trace() const
  {
    complex<double> out{};
    
    for(int ig1=0;ig1<4;ig1++)
      if(pos[ig1]==ig1)
	{
	  const double r=out.real()+entr[ig1].real();
	  const double i=out.imag()+entr[ig1].imag();
	  out={r,i};
	}
    
    return out;
  }
  
  constexpr dirac_matr iTimes(const double& d) const
  {
    dirac_matr out{};
    for(int ig1=0;ig1<4;ig1++)
      {
	out.pos[ig1]=pos[ig1];
	out.entr[ig1]={-d*entr[ig1].imag(),d*entr[ig1].real()};
      }
    
    return out;
  }
};

inline ostream &operator<<(ostream &os, const dirac_matr &m) {
  for(int i=0;i<4;i++)
    os<<m.pos[i]<<" "<<m.entr[i]<<" ";
  
  return os;
}

constexpr dirac_matr gam[16]=
  {dirac_matr{{0,1,2,3},{std::complex<double>{1,0},{1,0},{1,0},{1,0}}},
   {{3,2,1,0},{std::complex<double>{0,1},{0,1},{0,-1},{0,-1}}},
   {{3,2,1,0},{std::complex<double>{1,0},{-1,0},{-1,0},{1,0}}},
   {{2,3,0,1},{std::complex<double>{0,1},{0,-1},{0,-1},{0,1}}},
   {{0,1,2,3},{std::complex<double>{1,0},{1,0},{-1,0},{-1,0}}},
   {{2,3,0,1},{std::complex<double>{1,0},{1,0},{1,0},{1,0}}},
   gam[1]*gam[5],
   gam[2]*gam[5],
   gam[3]*gam[5],
   gam[4]*gam[5],
   gam[4]*gam[1],
   gam[4]*gam[2],
   gam[4]*gam[3],
   gam[2]*gam[3],
   gam[3]*gam[1],
   gam[1]*gam[2]};
