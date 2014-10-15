


#include <vgStableHeaders.h>
#include <vgSolar/vgsoSolarDirectionCalc.h>


namespace vgSolar {
	


	SolarDirectionCalc::SolarDirectionCalc( DateTimeAndLocation* time_loc)
	{
		assert( time_loc != NULL );
		setDateTimeAndLocation( *time_loc );
	}
	SolarDirectionCalc::~SolarDirectionCalc()
	{

	}
	void SolarDirectionCalc::setDate(int yearpar,int monthpar,int daypar)
	{
		year=yearpar;
		month=monthpar;
		day=daypar;
	}
	void SolarDirectionCalc::setTime(int hourpar,int minutepar)
	{
		hour=hourpar;
		minute=minutepar;
	}
	int SolarDirectionCalc::jiri(int year,int month,int day)
	{
		int leap;
		if ( (year%4)==0 &&(year%100!=0 )||(year%400==0) )
			leap=1;
		else 
			leap=0;
		int mm[12]={ 0,31,leap+59,leap+90,leap+120,leap+151,leap+181,leap+212,leap+243,leap+273,leap+304,leap+334};		
		return mm[month-1]+day;
	}

	float SolarDirectionCalc::GetLatOfSun()//����̫���ĳ�γ,�����̫����ֱ��γ���˰�,�����ǽǶ���ֵ
	{
		float t=jiri(year,month,day)-79.6764-0.2422*(year-1985)+floor((year-1985)/4.0);//���ն�����Դ��ֶ��ԣ�����֮ǰ�Ǹ���
		float sita=2*3.14159265*t/365.2422;//��ת��Ϊ����,365���Ӧ2pi�Ļ�����ô��������ڵĻ��ն�Ӧ�Ļ����ˣ���ʵ���ֶ�Ӧ�Ļ�����0
		float delta=0.3723+23.2567*sin(sita)+0.1149*sin(2*sita)-0.1712*sin(3*sita)-0.758*cos(sita)+0.3656*cos(2*sita)+0.0201*cos(3*sita);		
		return delta;
	}
	void SolarDirectionCalc::setLatitude(float latpar)
	{
		LatOfPos=latpar;
	}
	float SolarDirectionCalc::GetHeightOfSun()//������մ�ʱ�˵ص�̫���Ƕ�,Ҫ��ִ��SetDate,SetLat,SetTime
	{
		//cout<<"γ��"<<LatOfPos<<"�Ĵ�������̫�����Ƕ���"<<90-(LatOfPos-GetLatOfSun())<<endl;//�����ֵ�Ǹ������أ��Ǹ������˶�Ϊ��λ��С��

		float nowtime=hour+minute/60.0f;
		float timeangle=(nowtime-12)/12.0*3.14159265;//��Ϊ���ȵ�ʱ��
		//cout<<"ʱ��"<<timeangle<<endl;

		float LatOfSun=3.14159265*GetLatOfSun()/180.0;//��γ�ǶȻ���ɻ��Ȱ����źü���
		float RadLatOfPos=LatOfPos*3.14159265/180.0;//���ڵص�γ��
		//cout<<"sinh:"<<cos(RadLatOfPos)*cos(LatOfSun)*cos(timeangle)+sin(RadLatOfPos)*sin(LatOfSun)<<endl;
		float h=asin(cos(RadLatOfPos)*cos(LatOfSun)*cos(timeangle)+sin(RadLatOfPos)*sin(LatOfSun));//�õ�����
		if( h > 0.0f )
		{
			//cout<<"���մ�ʱ�˵ص�̫���Ƕ���"<<h*180/3.1415926<<endl;	
			//����Ҫ��������θ���̫����ʱ����Լ����ǻ������Ӧ��vec3�������Ұ���z��Ϊ���Ļ���
			float x,y,z;
			float aa=h;//���û��ȼ������Ǻ�����
			float t=timeangle;
			x=-cos(aa)*sin(t);
			y=sin(aa);
			z=cos(aa)*cos(t);
			//cout<<"���մ�ʱ�˵�ָ��̫����ʸ����"<<x<<" "<<y<<" "<<z<<endl;
			//������Ӧ�ø��������άʸ���趨̫���Ĺ��߽ǣ�Ȼ��ʼ��Ⱦ��Ӱ��
			return h;
		}
		else
		{
			//cout<<"̫����û��"<<endl;
			return h;
		}
	}
	//----------------------------------------------------------------
	vgKernel::Vec3* SolarDirectionCalc::getSolarDirection()
	{
		float hh = GetHeightOfSun();

		float nowtime= hour + minute/60.0f;
		float t=(nowtime-12.0f)/12.0f*3.14159265;//��Ϊ���ȵ�ʱ��

		vgKernel::Vec3* sun = new vgKernel::Vec3;

		if( hh > 0.0f )//̫��û���Ļ�����-1���㣡
		{
			sun->x=-cos(hh)*sin(t);
			sun->y=-sin(hh);
			sun->z=cos(hh)*cos(t);

			//sun.normalise();
			sun->normalise();

			return sun;
		}
		else
		{
			return NULL;
		}

	}
	//----------------------------------------------------------------

	
	

}// end of namespace vgSolar
