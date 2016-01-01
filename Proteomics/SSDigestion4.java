package SSDigestion4;

import java.awt.event.*;
import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Collections;
import java.util.Iterator;
import java.math.BigDecimal; //precision of double

public class SSDigestion4
{
	//H  1.00782   O  15.99492   C 12    N 14.00308
	// molecular wieght of amino acid from Alanine (A, index 0) to Tyrosine (Y, index 24)
	// aaMass is 2D array, one row for each amino acid in the sequence of A to Z
	// column 1 is onoisotopic residue mass
	// column 2 is average residue mass
	// column 3 is count
	static double[][] aaMass = new double[][]{
		{71.03711,71.08,0},{0,0,0},{103.00919,103.15,0},{115.02694,115.09,0},{129.04259,129.12,0},
		{147.06841,147.18,0},{57.02146,57.05,0},{137.05891,137.14,0},{113.08406,113.16,0},{0,0,0},
		{128.09496,128.17,0},{113.08406,113.16,0},{131.04049,131.2,0},{114.04293,114.1,0},{0,0,0},
		{97.05276,97.12,0},{128.05858,128.13,0},{156.10111,156.19,0},{87.03203,87.08,0},{101.04768,101.11,0},
		{0,0,0},{99.06841,99.13,0},{186.07931,186.21,0},{0,0,0},{163.06333,163.18,0},
	};
	static double mwCal(String str)
	{
		char ch;
		int i,j;
		double mw = 0;
		for (i = 0; i < str.length(); i++) 
		{
			ch = str.charAt(i);
			j = (int) ch - 65;
			aaMass[j][2]++;
		}
		for (i = 0; i < 25; i++) {
			mw = mw + aaMass[i][0] * aaMass[i][2];
			aaMass[i][2]=0;
		}
		mw += 18.01056; //molecular weight of N and C end H2O
		return mw;
	}
	static BigDecimal toBigDecimal(double mw)
	{
		BigDecimal bdmw=new BigDecimal(mw);
		bdmw=bdmw.setScale(5, BigDecimal.ROUND_HALF_UP);
		return bdmw;
	}
	/*public static void printElement(Collection<Integer> c)
    {
    	Iterator<Integer> it = c.iterator();
    	while(it.hasNext())
    	{
    		System.out.println(it.next());
    	}	
    }
     public static void printCysPosition(Collection<int[]> c)
    {
    	Iterator<int[]> it = c.iterator();
    	while(it.hasNext())
    	{
    		System.out.println(it.next()[0]);
    	}
    }*/
	
	/*
	 * @param ArrayList<CysProperty> alCysProperty :  Cystein residue position
	 * @param ArrayList<Integer> alCleavePoint :  Digestion cleaveage position
	 */
	ArrayList<CysPeptide> findalCysPeptide(ArrayList<CysProperty> alCysProperty,  ArrayList<Integer> alCleavePoint)
	{
		ArrayList<CysPeptide> alCysPeptide = new ArrayList<CysPeptide>();
		int cysPosition, cysPeptideNo=-1,i=0;
		boolean currentPeptide=false;   //�ж�����peptide��û�в�����CysPeptide��Ķ���
		for(int m=0;m<alCysProperty.size();m++)   //ȷ��ÿһ��Cys Residues����һ��Cys Peptide��,���Ұ�ÿһ��cysPeptide����alCysPeptide
		{	
			cysPosition=alCysProperty.get(m).position; //��ȡalCysProperty�е�Ԫ��
			while(i<alCleavePoint.size())
			{
				if(cysPosition<=alCleavePoint.get(i))  //���C terminus���һ��residue��Cys,������=�������������������С�ڵ���
				{           			
					CysPeptide Cp;  			
					if(currentPeptide==false)
					{
						Cp=new CysPeptide();
						cysPeptideNo++;
						currentPeptide=true;
						if(i==0)
						{
							Cp.start=1;
						}
						else Cp.start=alCleavePoint.get(i-1)+1;
						Cp.end=alCleavePoint.get(i);
						alCysPeptide.add(Cp);
						Cp.alCysPositionInPeptide.add(cysPosition);
					}
					else
					{
						alCysPeptide.get(alCysPeptide.size()-1).alCysPositionInPeptide.add(cysPosition); //�����һ��������CysPeptide���е�CysPositionInPeptideд�룬
						//�������project,ִ�е�elseʱ��Cp����϶��Ѿ������ˣ�currentPeptide=true),���﷨�ϲ�����else����е���if����ж����Cp����������get��alCysPeptide.size())����д��
						//System.out.println("Cys84 is found and added into ArrayList<Integer> alCysPositionInPeptide");
					}
					alCysProperty.get(m).cysPeptideNo=cysPeptideNo; //��cysProperty�ĵ�����ֵ��ֵ����Cys�ڵڼ���cysPeptide�ϣ���0��ʼ��
					break; //ֻҪ�ҵ�cysPosition<(int)objCleavePoint[i]����������ǰwhileѭ�����ж���һ��Cys Residue
				}
				i++;
				currentPeptide=false;  //�����п���λ�ڴ�Cys Peptide�ϵ�Cys������Ժ�������һ��Peptide��i++)ǰ������һ��Peptide��label��Ϊfalse           			         		
			}
		}
		return alCysPeptide;
	}
	ArrayList<alSSPeptide> findalSSPeptide(String sequence, ArrayList<CysProperty> alCysProperty,  ArrayList<CysPeptide> alCysPeptide) //����enzyme cleavage site��SS connectivity��Ϣ���õ�SSPeptide�ķ���
	{
		ArrayList<alSSPeptide> alSSPeptides=new ArrayList<alSSPeptide>(); //size����������ȷ���Ķ�ά��������    
		int i;
		for(i=0;i<alCysPeptide.size();i++)
		{
			if(alCysPeptide.get(i).sequenceCounted==false)
			{
				ArrayList<Integer> alSSPeptide=new ArrayList<Integer>();
				alSSPeptide.add(i);  //��0��CysPeptide ����SSPeptide
				alCysPeptide.get(i).sequenceCounted=true; 
				for(int k=0;k<alSSPeptide.size();k++)  //alSSPeptide.size()��̬�ı仯
				{         			
					for(int j=0;j<alCysPeptide.get(alSSPeptide.get(k)).alCysPositionInPeptide.size();j++)  //CysPeptide�ϵ�ÿ��Cys
					{
						for(int m=0;m<alCysProperty.size();m++)
						{
							if(alCysProperty.get(m).cysPeptideChecked==true)
							{
								continue;
							}
							else if(alCysProperty.get(m).position==alCysPeptide.get(alSSPeptide.get(k)).alCysPositionInPeptide.get(j))  //ͨ��ѭ���ҵ��ҵ�Cys residue���ڵ�һά����index m
							{
								for(int n=0;n<alCysProperty.size();n++)  //�ҵ�Cys Pair���ڵ�һά����index
								{
									if(alCysProperty.get(n).position==alCysProperty.get(m).partnerPosition)
									{
										alCysProperty.get(m).cysPeptideChecked=true; //��һ��disulfide���ӵ�����Cys�Ѿ���check��
										alCysProperty.get(n).cysPeptideChecked=true;
										if(alCysPeptide.get(alCysProperty.get(n).cysPeptideNo).sequenceCounted==false)//����������µ�peptide
										{
											alSSPeptide.add(alCysProperty.get(n).cysPeptideNo);  //�ҵ���Cys Pair���ڵ�peptide index
											//System.out.println(alSSPeptide.size());
											alCysPeptide.get(alCysProperty.get(n).cysPeptideNo).sequenceCounted=true;		            							
											break;   //����if(cysProperty[n][0]==cysProperty[m][1])��n���ҵ���������n��ѭ��
										}
									}
								}
								break; //����else if��m���ҵ�����������m��ѭ��
							}
						}
					}
				}
				int start, end, cysNo,op;
				double subMass,totalMass=0;
				for(op=0;op<alSSPeptide.size();op++)  //����ÿһ��alSSPeptide��mass,Ϊ������׼��
				{           			
					start=alCysPeptide.get(alSSPeptide.get(op)).start;
					end=alCysPeptide.get(alSSPeptide.get(op)).end;
					cysNo=alCysPeptide.get(alSSPeptide.get(op)).alCysPositionInPeptide.size();
					subMass=mwCal(sequence.substring(start-1, end))-1.00782*cysNo;
					totalMass=totalMass+subMass;            		
				}
				Collections.sort(alSSPeptide); //�Ѵ���alSSPeptide�ϵ�CysPeptideNo���򣬷���remove repeat
				alSSPeptide instanceofalSSPeptide=new alSSPeptide();
				instanceofalSSPeptide.alSSPeptide=alSSPeptide;
				instanceofalSSPeptide.totalMass=totalMass; 
				alSSPeptides.add(instanceofalSSPeptide);
			}
		}
		return alSSPeptides;
	}
	public static void main(String[] args)
	{   
		// JPanel
		JLabel l = new JLabel("Please input the sequence:");
		JLabel l2 = new JLabel("Please indicate the SS linkages:");
		JLabel l3 = new JLabel("Please choose enzyme:");
		final JTextField tf = new JTextField(200);
		final JTextField tfSS = new JTextField();
		final JTextField tfEnzyme = new JTextField();
		JPanel jp = new JPanel(); //flowlayout
		jp.setLayout(new GridLayout(6,1));
		jp.add(l);
		jp.add(tf);
		jp.add(l2);
		jp.add(tfSS);
		jp.add(l3);
		jp.add(tfEnzyme);
		// JFrame
		JFrame jf = new JFrame("Disulfide Protein Digest");
		jf.setSize(600,400);
		jf.setLocation(100,100);
		jf.getContentPane().add(jp,"North");
		final JTextArea ta = new JTextArea();
		jf.getContentPane().add(ta,"Center");
		JButton btn = new JButton("Submit");
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
				String input = tf.getText().trim();
				String sequence;
				String strSS;   // e.g., "26-84,40-95,58-110,65-72"
				if(input.compareTo("RNase A") == 0)
				{
					sequence = "KETAAAKFERQHMDSSTSAASSSNYCNQMMKSRNLTKDRCKPVNTFVHESLADVQAVCSQKNVACKNGQTNCYQSYSTMSITDCRETGSSKYPNCAYKTTQANKHIIVACEGNPYVPVHFDASV";//Ribonuclease A
					strSS = "26-84,40-95,58-110,65-72"; //RNase A
				}
				else if(input.compareTo("BSA") == 0)
				{
					sequence = "DTHKSEIAHRFKDLGEEHFKGLVLIAFSQYLQQCPFDEHVKLVNELTEFAKTCVADESHAGCEKSLHTLFGDELCKVASLRETYGDMADCCEKQEPERNECFLSHKDDSPDLPKLKPDPNTLCDEFKADEKKFWGKYLYEIARRHPYFYAPELLYYANKYNGVFQECCQAEDKGACLLPKIETMREKVLTSSARQRLRCASIQKFGERALKAWSVARLSQKFPKAEFVEVTKLVTDLTKVHKECCHGDLLECADDRADLAKYICDNQDTISSKLKECCDKPLLEKSHCIAEVEKDAIPENLPPLTADFAEDKDVCKNYQEAKDAFLGSFLYEYSRRHPEYAVSVLLRLAKEYEATLEECCAKDDPHACYSTVFDKLKHLVDEPQNLIKQNCDQFEKLGEYGFQNALIVRYTRKVPQVSTPTLVEVSRSLGKVGTRCCTKPESERMPCTEDYLSLILNRLCVLHEKTPVSEKVTKCCTESLVNRRPCFSALTPDETYVPKAFDEKLFTFHADICTLPDTEKQIKKQTALVELLKHKPKATEEQLKTVMENFVAFVDKCCAADDKEACFAVEGPKLVVSTQTALA";//BSA
					strSS = "53-62,75-91,90-101,123-168,167-176,199-245,244-252,264-278,277-288,315-360,359-368,391-437,436-447,460-476,475-486,513-558,557-566";//BSA  
				}
				else if(input.compareTo("Lysozyme") == 0)
				{
					sequence = "KVFGRCELAAAMKRHGLDNYRGYSLGNWVCAAKFESNFNTQATNRNTDGSTDYGILQINSRWWCNDGRTPGSRNLCNIPCSALLSSDITASVNCAKKIVSDGNGMNAWVAWRNRCKGTDVQAWIRGCRL";//chicken lysozyme
					strSS = "6-127,30-115,64-80,76-94"; //chicken lysozyme
				}
				else 
				{
					sequence=input;
					strSS = tfSS.getText().trim();
				}
				//String line = System.getProperty("line.separator");
				//Parse input sequence
				String ssPair[] = strSS.split(",");
				ArrayList<CysProperty> alCysProperty = new ArrayList<CysProperty>();
				String[] indexPair = new String[2];
				for (int i = 0; i < ssPair.length; i++) {
					indexPair = ssPair[i].split("-");
					CysProperty CProperty1 = new CysProperty();
					CysProperty CProperty2 = new CysProperty();
					CProperty1.position = Integer.parseInt(indexPair[0]);
					CProperty1.partnerPosition = Integer.parseInt(indexPair[1]);
					CProperty2.position = Integer.parseInt(indexPair[1]);
					CProperty2.partnerPosition = Integer.parseInt(indexPair[0]);
					alCysProperty.add(CProperty1);
					alCysProperty.add(CProperty2);
				}
				// sort CycProperty by position
				class CysPositionComparator implements Comparator<CysProperty>
				{
					public int compare(CysProperty A, CysProperty B)
					{
						int result = A.position > B.position ? 1 : -1;
						return result;
					}
				}
				Collections.sort(alCysProperty, new CysPositionComparator());
				//printCysPosition(alCysProperty);   // test
				int fromIndex = 0;
				int i;
				String enzyme = tfEnzyme.getText();
				ArrayList<Integer> alCleavePoint = new ArrayList<Integer>();
				if(enzyme.compareTo("Trypsin") == 0)
				{
					i = 0;
					char c;
					while (i<sequence.length()) 
					{ 
						c = sequence.charAt(i);
						if(c == 'K' || c == 'R')
						{
							alCleavePoint.add(i + 1);
						}
						i++;
					}
					c = sequence.charAt(i-1);
					if(c != 'K' && c != 'R') //peptide in C-end
						alCleavePoint.add(i);
				}
				else // Aspartic acid
				{
					while ((i = sequence.indexOf("E", fromIndex)) != -1) 
					{ 
						alCleavePoint.add(i + 1);
						fromIndex = i + 1;
					}
					if(fromIndex<sequence.length()) //peptide in C-end
					{
						alCleavePoint.add(sequence.length());
					}
				}
				//printElement(alCleavePoint);   // test
				//Print Cys position in each CysPeptide        
				ArrayList<CysPeptide> alCysPeptide;
				ArrayList<alSSPeptide> alSSPeptides; //dynamic 2D array
				
				// Parsing done, Begin execution
				SSDigestion4 ActualSSDigestion4 = new SSDigestion4();
				alCysPeptide = ActualSSDigestion4.findalCysPeptide(alCysProperty, alCleavePoint);
				alSSPeptides = ActualSSDigestion4.findalSSPeptide(sequence, alCysProperty, alCysPeptide);
				Iterator<CysPeptide> itCysPeptide=alCysPeptide.iterator();
				while(itCysPeptide.hasNext())
				{
					//System.out.println(itCysPeptide.next().start);
					Iterator<Integer> itCysPositionInPeptide=itCysPeptide.next().alCysPositionInPeptide.iterator();
					while(itCysPositionInPeptide.hasNext())
					{
						System.out.print(itCysPositionInPeptide.next()+"  ");
					}
					System.out.println();
				}   
				alSSPeptide alSSPeptide1=new alSSPeptide();
				ArrayList<Integer> alSSPeptide=new ArrayList<Integer>();
				alSSPeptide.add(0);
				alSSPeptide.add(3);
				alSSPeptide1.alSSPeptide=alSSPeptide;
				alSSPeptides.add(alSSPeptide1);
				//remove repeated alsspeptide that are generated by a different digest with a different miss cleavage setting      
				int j;
				for(i=0;i<alSSPeptides.size();i++)
				{                	           	  
					for(j=i+1;j<alSSPeptides.size();j++)
					{                		 
						if(alSSPeptides.get(i).alSSPeptide.equals(alSSPeptides.get(j).alSSPeptide))
						{
							alSSPeptides.remove(j);
							j--;     
						}
					}
				}
				//��alSSPeptides��totalMass����
				class alSSPeptidesComparator implements Comparator<alSSPeptide>
				{
					public int compare(alSSPeptide a, alSSPeptide b)
					{
						int result=a.totalMass>b.totalMass ? 1 : -1;
						return result;
					}
				}
				Collections.sort(alSSPeptides, new alSSPeptidesComparator());     
				//ArrayList<Integer> alSSPeptide=new ArrayList<Integer>();
				int op,start,end,cysNo;
				double subMass;
				for(i=0;i<alSSPeptides.size();i++)
				{
					alSSPeptide=alSSPeptides.get(i).alSSPeptide;
					for(op=0;op<alSSPeptide.size();op++)
					{       			
						if(op>0)
						{
							System.out.print(" ");
						}
						start=alCysPeptide.get(alSSPeptide.get(op)).start;
						end=alCysPeptide.get(alSSPeptide.get(op)).end;
						System.out.print(start+"-"+end);	
					}
					System.out.println();
					for(op=0;op<alSSPeptide.size();op++)
					{
						if(op>0)
						{
							System.out.print(" ");
						}
						start=alCysPeptide.get(alSSPeptide.get(op)).start;
						end=alCysPeptide.get(alSSPeptide.get(op)).end;
						System.out.print(sequence.substring(start-1, end));	
					}
					System.out.println();
					for(op=0;op<alSSPeptide.size();op++)
					{
						if(op>0)
						{
							System.out.print("  +  ");
						}
						start=alCysPeptide.get(alSSPeptide.get(op)).start;
						end=alCysPeptide.get(alSSPeptide.get(op)).end;
						cysNo=alCysPeptide.get(alSSPeptide.get(op)).alCysPositionInPeptide.size();
						subMass=mwCal(sequence.substring(start-1, end))-1.00782*cysNo;
						System.out.print(toBigDecimal(subMass)+"cysNo: ("+cysNo+") ");	
					}
					System.out.print("  =  "+toBigDecimal(alSSPeptides.get(i).totalMass));
					System.out.println();
				}
				System.out.println("All possible disulfide-linked peptides:   "+alSSPeptides.size()); 
				//ta.append("Molecular Weight:   "+mwCal(sequence)+line);
			}
		});
	}
}

class CysProperty
{
	int position, partnerPosition, cysPeptideNo;
	//cysProperty[0]:position in sequence,[1]:partner's position,[2]in which cysPeptide��[3]during search of SSPeptide��mark whether this disulfide has been checked
	boolean cysPeptideChecked = false;
}

class CysPeptide
{
	int start, end;
	ArrayList<Integer> alCysPositionInPeptide = new ArrayList<Integer>();
	boolean sequenceCounted = false;
}

class alSSPeptide
{
	ArrayList<Integer> alSSPeptide;
	ArrayList<Integer> alStart;
	ArrayList<Integer> alEnd;
	double totalMass;
}