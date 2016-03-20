import java.awt.event.*;
import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.math.BigDecimal; 

//consider miss cleavage

public class AntibodyAllPossible
{
	//H  1.00782   O  15.99492   C 12    N 14.00308
   static double[][] aaMass=new double[][]{
    {71.03711,71.08,0},{0,0,0},{103.00919,103.15,0},{115.02694,115.09,0},{129.04259,129.12,0},
    {147.06841,147.18,0},{57.02146,57.05,0},{137.05891,137.14,0},{113.08406,113.16,0},{0,0,0},
    {128.09496,128.17,0},{113.08406,113.16,0},{131.04049,131.2,0},{114.04293,114.1,0},{0,0,0},
    {97.05276,97.12,0},{128.05858,128.13,0},{156.10111,156.19,0},{87.03203,87.08,0},{101.04768,101.11,0},
    {0,0,0},{99.06841,99.13,0},{186.07931,186.21,0},{0,0,0},{163.06333,163.18,0},   //25排2列
    };
    static double mwCal(String str)
          {
            char ch;
            int i,j;
            double mw = 0;
            for (i = 0; i < str.length(); i++) {
              ch = str.charAt(i);
              j = (int) ch - 65;
              aaMass[j][2]++;
            }
            for (i = 0; i < 25; i++) {
              mw = mw + aaMass[i][0] * aaMass[i][2];
              aaMass[i][2]=0;
            }
            mw = mw + 18.01056; //N端和C端一分子水
            return mw;
          }
    static BigDecimal toBigDecimal(double mw)
    {
    	BigDecimal bdmw=new BigDecimal(mw);
    	bdmw=bdmw.setScale(5, BigDecimal.ROUND_HALF_UP);
        return bdmw;
    }
    public static void printElement(Collection<Integer> c)
    {
    	Iterator<Integer> it=c.iterator();
    	while(it.hasNext())
    	{
    		System.out.println(it.next());
    	}	
    }  
    int beginIndex=1;
    String trypsinPeptide;
    CysPeptide cysPeptide=new CysPeptide(); 
    void findalCysPeptide(String sequence, ArrayList<CysPeptide> alCysPeptide, ArrayList<Integer> alCleavePoint)
    {
    	 for(int i=0;i<alCleavePoint.size();i++)  //Trypsin digestion
         {
       	  trypsinPeptide=sequence.substring(beginIndex-1,alCleavePoint.get(i));
       	  if(trypsinPeptide.indexOf('C')!=-1) //判断此trypsin peptide是否为cysPeptide
   		  {
   			  cysPeptide.sequence=trypsinPeptide;
   			  cysPeptide.beginIndex=beginIndex;
   			  cysPeptide.endIndex=alCleavePoint.get(i);
   			  alCysPeptide.add(cysPeptide);
   			  //System.out.println(sequence.substring(cysPeptide.beginIndex-1,cysPeptide.endIndex));//检查正确   
   			  cysPeptide=new CysPeptide();   			          			 
   		  }
       	  beginIndex=alCleavePoint.get(i)+1;
         }
    	 
    }
    ArrayList<Integer> tmpCleavePoint=new ArrayList<Integer>();
    void count(int i,ArrayList<Integer> tmpCleavePoint, ArrayList<Integer> alCleavePoint,int n) 
    {
    	ArrayList<Integer> tmp=new ArrayList<Integer>();
    if(n==0){
        for(int j=0;j<tmpCleavePoint.size();j++)
        {
        	System.out.print(tmpCleavePoint.get(j)+"  ");
        }
        System.out.println();
        return;
    }
    if(i==alCleavePoint.size()){
        return;
    }
    count(i+1,tmpCleavePoint,alCleavePoint,n);
    tmpCleavePoint.add(alCleavePoint.get(i));
    count(i+1,tmpCleavePoint,alCleavePoint,n-1);
    }
  public static void main(String[] args)
  {
    
    JFrame jf=new JFrame("Disulfide Protein Digest");
    jf.setSize(600,400);
    jf.setLocation(100,100);
    JLabel l1=new JLabel("Please indicate the lihgt chain sequence:");
    JLabel l2=new JLabel("Please indicate intra SS bonds in light chain:");
    JLabel l3=new JLabel("Please indicate the heavy chain sequence:");
    JLabel l4=new JLabel("Please indicate intra SS bonds in heavy chain:");
    JLabel l5=new JLabel("Please indicate inter SS bonds between light and heavy chain (light-heavy):");
    JLabel l6=new JLabel("Please indicate inter SS bonds between two heavy chains:");
    JLabel l7=new JLabel("Please choose enzyme:");
    final JTextField tfLight=new JTextField("GQPKANPTVTLFPPSSEELQANKATLVCLISDFYPGAVTVAWKADGSPVKAGVETTKPSKQSNNKYAASSYLSLTPEQWKSHRSYSCQVTHEGSTVEKTVAPTECS");
    final JTextField tfHeavy=new JTextField("ASTKGPSVFPLAPSSKSTSGGTAALGCLVKDYFPEPVTVSWNSGALTSGVHTFPAVLQSSGLYSLSSVVTVPSSSLGTQTYICNVNHKPSNTKVDKKVEPKSCDKTHTCPPCPAPELLGGPSVFLFPPKPKDTLMISRTPEVTCVVVDVSHEDPEVKFNWYVDGVEVHNAKTKPREEQYNSTYRVVSVLTVLHQDWLNGKEYKCKVSNKALPAPIEKTISKAKGQPREPQVYTLPPSRDELTKNQVSLTCLVKGFYPSDIAVEWESNGQPENNYKTTPPVLDSDGSFFLYSKLTVDKSRWQQGNVFSCSVMHEALHNHYTQKSLSLSPGK");
    final JTextField tfSSIntraLight=new JTextField("28-87");
    final JTextField tfSSIntraHeavy=new JTextField("27-83,144-204,250-308");
    final JTextField tfSSInterLH=new JTextField("105-103");
    final JTextField tfSSInterHH=new JTextField("109-109,112-112");
    final JTextField tfEnzyme=new JTextField("Trypsin");
    JPanel jp=new JPanel(); //flowlayout
    jp.setLayout(new GridLayout(14,1));
    jp.add(l1);
    jp.add(tfLight);
    jp.add(l2);
    jp.add(tfSSIntraLight);
    jp.add(l3);
    jp.add(tfHeavy);
    jp.add(l4);
    jp.add(tfSSIntraHeavy);
    jp.add(l5);
    jp.add(tfSSInterLH);
    jp.add(l6);
    jp.add(tfSSInterHH);
    jp.add(l7);
    jp.add(tfEnzyme);
    jf.getContentPane().add(jp,"North");
    final JTextArea ta=new JTextArea();
    jf.getContentPane().add(ta,"Center");
    JButton btn=new JButton("Submit");
    jf.getContentPane().add(btn,"South");
    jf.setVisible(true);
    jf.addWindowListener(new WindowAdapter()
    {
      public void windowClosing(WindowEvent e)
      {
        System.exit(0);
      }
    });
    btn.addActionListener(new ActionListener()
    {
          public void actionPerformed(ActionEvent e)
          {     	
            String strLight = tfLight.getText(),
            	   strHeavy = tfHeavy.getText(),
                   strSSIntraLight = tfSSIntraLight.getText(),
                   strSSIntraHeavy = tfSSIntraHeavy.getText(),
                   strSSInterLH = tfSSInterLH.getText(),
                   strSSInterHH = tfSSInterHH.getText();
            //String sequence=strLight+strHeavy+strHeavy+strLight;  
            //Test
            String sequence=strLight;
            //String line = System.getProperty("line.separator");
            class CysProperty
            {
            	int position, partnerPosition, cysPeptideNo;
            	 //cysProperty[0]:position in sequence,[1]:partner's position,[2]在第几条cysPeptide上，[3]在搜索SSPeptide时，判断这条disulfide有没有被检查过
            	boolean cysPeptideChecked=false;
            }
            ArrayList<CysProperty> alCysProperty=new ArrayList<CysProperty>();
            String[] ssPair;
            ssPair = strSSIntraLight.split(",");
            String[] indexPair=new String[2];
            for (int i = 0; i < ssPair.length; i++) {
                indexPair = ssPair[i].split("-");
                CysProperty CProperty1=new CysProperty();
                CysProperty CProperty2=new CysProperty();
                CProperty1.position = Integer.parseInt(indexPair[0]); //第i对pair中的两个数放入cysPrpoerty的i*2和i*2+1位置
                CProperty1.partnerPosition = Integer.parseInt(indexPair[1]);
                CProperty2.position = Integer.parseInt(indexPair[1]);
                CProperty2.partnerPosition = Integer.parseInt(indexPair[0]);
                alCysProperty.add(CProperty1);
                alCysProperty.add(CProperty2);
              }  
            /* Test for a single chain protein such as BSA
            //SSIntraLight L1L1 and L2L2
            for (int i = 0; i < ssPair.length; i++) {
                indexPair = ssPair[i].split("-");
                CysProperty CProperty1=new CysProperty();
                CysProperty CProperty2=new CysProperty();
                CysProperty CProperty3=new CysProperty();
                CysProperty CProperty4=new CysProperty();
                CProperty1.position = Integer.parseInt(indexPair[0]); //第i对pair中的两个数放入cysPrpoerty的i*2和i*2+1位置
                CProperty1.partnerPosition = Integer.parseInt(indexPair[1]);
                CProperty2.position = Integer.parseInt(indexPair[1]);
                CProperty2.partnerPosition = Integer.parseInt(indexPair[0]);
                CProperty3.position = Integer.parseInt(indexPair[0])+strLight.length()+strHeavy.length()*2; //第i对pair中的两个数放入cysPrpoerty的i*2和i*2+1位置
                CProperty3.partnerPosition = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length()*2;
                CProperty4.position = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length()*2;
                CProperty4.partnerPosition = Integer.parseInt(indexPair[0])+strLight.length()+strHeavy.length()*2;
                alCysProperty.add(CProperty1);
                alCysProperty.add(CProperty2);
                alCysProperty.add(CProperty3);
                alCysProperty.add(CProperty4);
              } 
            //SSIntraHeavy  H1H1 and H2H2
            if(strSSIntraHeavy.length()>1)
            {           	
            	 ssPair = strSSIntraHeavy.split(",");
                 for (int i = 0; i < ssPair.length; i++) 
                 {
                     indexPair = ssPair[i].split("-");
                     CysProperty CProperty1=new CysProperty();
                     CysProperty CProperty2=new CysProperty();
                     CysProperty CProperty3=new CysProperty();
                     CysProperty CProperty4=new CysProperty();
                     CProperty1.position  = Integer.parseInt(indexPair[0])+strLight.length();
                     CProperty1.partnerPosition = Integer.parseInt(indexPair[1])+strLight.length();
                     CProperty2.position = Integer.parseInt(indexPair[1])+strLight.length();
                     CProperty2.partnerPosition = Integer.parseInt(indexPair[0])+strLight.length();
                     CProperty3.position  = Integer.parseInt(indexPair[0])+strLight.length()+strHeavy.length();
                     CProperty3.partnerPosition = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length();
                     CProperty4.position = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length();
                     CProperty4.partnerPosition = Integer.parseInt(indexPair[0])+strLight.length()+strHeavy.length();
                     alCysProperty.add(CProperty1);
                     alCysProperty.add(CProperty2);
                     alCysProperty.add(CProperty3);
                     alCysProperty.add(CProperty4);
                 }
            }
            //SSInterL1H1 and SSInterL2H2
            if(strSSInterLH.length()>1)
            {
                ssPair = strSSInterLH.split(",");
                for (int i = 0; i < ssPair.length; i++) 
                {
                    indexPair = ssPair[i].split("-");
                    CysProperty CProperty1=new CysProperty();
                    CysProperty CProperty2=new CysProperty();
                    CysProperty CProperty3=new CysProperty();
                    CysProperty CProperty4=new CysProperty();
                    CProperty1.position  = Integer.parseInt(indexPair[0]);
                    CProperty1.partnerPosition = Integer.parseInt(indexPair[1])+strLight.length();
                    CProperty2.position = Integer.parseInt(indexPair[1])+strLight.length();
                    CProperty2.partnerPosition = Integer.parseInt(indexPair[0]);
                    CProperty3.position = Integer.parseInt(indexPair[0])+strLight.length()+strHeavy.length()*2; //第i对pair中的两个数放入cysPrpoerty的i*2和i*2+1位置
                    CProperty3.partnerPosition = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length();
                    CProperty4.position = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length();
                    CProperty4.partnerPosition = Integer.parseInt(indexPair[0])+strLight.length()+strHeavy.length()*2;
                    alCysProperty.add(CProperty1);
                    alCysProperty.add(CProperty2);
                    alCysProperty.add(CProperty3);
                    alCysProperty.add(CProperty4);
                }   
            }
            //SSInterH1H2
            if(strSSInterHH.length()>1)
            {
            	ssPair = strSSInterHH.split(",");
                for (int i = 0; i < ssPair.length; i++) 
                {
                    indexPair = ssPair[i].split("-");
                    CysProperty CProperty1=new CysProperty();
                    CysProperty CProperty2=new CysProperty();
                    CProperty1.position  = Integer.parseInt(indexPair[0])+strLight.length();
                    CProperty1.partnerPosition = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length();
                    CProperty2.position = Integer.parseInt(indexPair[1])+strLight.length()+strHeavy.length();
                    CProperty2.partnerPosition = Integer.parseInt(indexPair[0])+strLight.length();
                    alCysProperty.add(CProperty1);
                    alCysProperty.add(CProperty2);
                }
            }
            */
            class CysPositionComparator implements Comparator<CysProperty>
            {
            	public int compare(CysProperty A, CysProperty B)
            	{
            		int result=A.position>B.position ? 1 : -1;
            		return result;
            	}
            }
            Collections.sort(alCysProperty, new CysPositionComparator());
            //System.out.println("Print cysPositioin:");
            /*Iterator<CysProperty> it=alCysProperty.iterator();
            CysProperty cp=new CysProperty();
            while(it.hasNext())
            {
            	cp=it.next();
                System.out.println(cp.position+"-"+cp.partnerPosition);
            }
            System.out.println("End");*/
            int fromIndex = 0;
            int i;
            String enzyme=tfEnzyme.getText();
            ArrayList<Integer> alCleavePoint=new ArrayList<Integer>();
            if(enzyme.compareTo("Trypsin")==0)
            {
                i=0;
                char c;
                while (i<sequence.length()) { //75是K的ASCII码,69是E
                    c=sequence.charAt(i);
                    if(c=='K'||c=='R')
                	{
                    	alCleavePoint.add(i+1); 
                	}
                    i++;
                };
                c=sequence.charAt(i-1); //全部循环一遍后，判断最后一个residue
                if(c!='K'&c!='R')
                	alCleavePoint.add(i);
            }
            else //Glu C digestion
            {
	            while ( (i = sequence.indexOf("E", fromIndex)) != -1) { //75是K的ASCII码,69是E,82是R
	              alCleavePoint.add(i+1);
	              fromIndex = i + 1;
	            }
	            if(fromIndex<sequence.length())
	            {
	            	alCleavePoint.add(sequence.length());
	            }
            }
            //add cleavage at each chain  
            /* block this for BSA, use this for antibody
            alCleavePoint.add(strLight.length());
            alCleavePoint.add(strLight.length()+strHeavy.length());
            alCleavePoint.add(strLight.length()+strHeavy.length()*2);*/
            //if the last residue in one chain is K or E, should I remove repeated point????
            class CleavePointComparator implements Comparator<Integer>
            {
            	public int compare(Integer a, Integer b)
            	{
            		int result=a>b ? 1 : -1;
            		return result;
            	}
            }
            Collections.sort(alCleavePoint, new CleavePointComparator());
            /*for(i=0;i<alCleavePoint.size();i++)
            {
            	System.out.println(alCleavePoint.get(i));
            }*/
            /*printElement(alCleavePoint);     
            System.out.println("End printing alCleavePoint");*/
            ArrayList<CysPeptide> alCysPeptide=new ArrayList<CysPeptide>();
            ArrayList<SSPeptide> alSSPeptide=new ArrayList<SSPeptide>();
            SSPeptide ssPeptide=new SSPeptide();
            AntibodyAllPossible ActualSSDigestion=new AntibodyAllPossible();
            ActualSSDigestion.findalCysPeptide(sequence, alCysPeptide, alCleavePoint);
            ArrayList<Integer> tmpCleavePoint=new ArrayList<Integer>();
            ActualSSDigestion.count(0, tmpCleavePoint, alCleavePoint, 1);

            //Print Cys position in each CysPeptide
            /*Iterator<CysPeptide> itCysPeptide=alCysPeptide.iterator();
            CysPeptide cp=new CysPeptide();
            while(itCysPeptide.hasNext())
            {
            	cp=itCysPeptide.next();
            	System.out.println(sequence.substring(cp.beginIndex-1,cp.endIndex));
            }*/ 
            System.out.println("alCysPeptide.size() before removing repeated cyspeptide:"+alCysPeptide.size());
            int j;
            //remove repeated cyspeptide within a single protein
            for(i=0;i<alCysPeptide.size();i++)
            {                	           	  
          	  for(j=i+1;j<alCysPeptide.size();j++)
          	  {                		 
          		  if(alCysPeptide.get(i).sequence.equals(alCysPeptide.get(j).sequence))
          		  {
          			  alCysPeptide.remove(j);
          			  j--;     
          		  }
          	  }
            }
            System.out.println("alCysPeptide.size() after removing repeated cyspeptide:"+alCysPeptide.size());
            for(i=0;i<alCysPeptide.size();i++)  //add all possible SSPeptides in a single protein to alSSPeptide including dimer of the same cysPeptide
            {
          	  for(j=i;j<alCysPeptide.size();j++)
          	  {
          		  ssPeptide.SPeptideA=alCysPeptide.get(i);               			  
  				  ssPeptide.SPeptideB=alCysPeptide.get(j);
  				  ssPeptide.massSSPeptide=mwCal(ssPeptide.SPeptideA.sequence)+mwCal(ssPeptide.SPeptideB.sequence)-1.00782*2;
  				  /*if(ssPeptide.massSSPeptide<SS_HIGH_MASS_CUT_OFF)
  				  {
  					  alSSPeptide.add(ssPeptide);
  				  }*/
  				  alSSPeptide.add(ssPeptide);
  				  //System.out.println(ssPeptide.SPeptideA.sequence+"+"+ssPeptide.SPeptideB.sequence+" mass is  "+ssPeptide.massSSPeptide);
  				  ssPeptide=new SSPeptide();
          	  }
            } 	     		               
            System.out.println("alSSPeptide.size() = "+alSSPeptide.size());
            class SSPeptideMassComparator implements Comparator<SSPeptide>
            {
            	public int compare(SSPeptide A, SSPeptide B)
            	{
            		int result=A.massSSPeptide>B.massSSPeptide ? 1 : -1;
            		return result;
            	}
            }
            Collections.sort(alSSPeptide, new SSPeptideMassComparator());
            CysPeptide cysPeptideA=new CysPeptide(); 
            CysPeptide cysPeptideB=new CysPeptide(); 
            //Print alSSPeptide
            /*
            for(i=0;i<alSSPeptide.size();i++)
            {
            	cysPeptideA=alSSPeptide.get(i).SPeptideA;
            	cysPeptideB=alSSPeptide.get(i).SPeptideB;
            	System.out.println(cysPeptideA.beginIndex+"-"+cysPeptideA.endIndex+"  "
            	                +cysPeptideB.beginIndex+"-"+cysPeptideB.endIndex   );
            	System.out.println(cysPeptideA.sequence+"  "+cysPeptideB.sequence);
            	System.out.println((mwCal(cysPeptideA.sequence)-1.00782)+"  +  "+(mwCal(cysPeptideB.sequence)-1.00782)+
            	"  =  "+alSSPeptide.get(i).massSSPeptide);
            	
            }*/
            //ta.append("Molecular Weight:   "+mwCal(sequence)+line);
          }
    });
  }
}

class CysPeptide
{
	  String sequence;
	  int beginIndex, endIndex;     // for a peptide GGGKGGKGG, trypsin degest beginIndexk=1 or 5,endIndex=4 or 7
	  int missCleavage;
}
class SSPeptide
{
	  CysPeptide SPeptideA, SPeptideB;
	  double massSSPeptide;
}
