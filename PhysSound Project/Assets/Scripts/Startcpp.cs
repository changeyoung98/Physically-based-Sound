using UnityEngine;
using System.Collections;
using System.Threading;
using System.Runtime.InteropServices;
public class Startcpp : MonoBehaviour
{
    //  public int youngs;
    //  public double rho;
    //  private int force;
        public bool gen = false;
    public int num;
    public int mat;
    private Rigidbody _r;
    //  [DllImport("1D_5 Points")]
    //   private static extern int Precompute(double rhoD);
    [DllImport("gen_sound")]
    //   private static extern int USound(double rho, int youngs, int force);//, int num, double[] omegaD, double[] c, double[] mass, string[] UTstr, double[] frequencies);
    private static extern int USound(int obj,int mat,double force);
        // Use this for initialization double youngs,
    void Start()
    {
       // Debug.Log(USound(3000));
        _r = GetComponent<Rigidbody>();
    //    Precompute(rho);
    /*    TextAsset txt = Resources.Load("pre") as TextAsset;
   
        // 以换行符作为分割点，将该文本分割成若干行字符串，并以数组的形式来保存每行字符串的内容
        string[] str = txt.text.Split('\n');

        // 将每行字符串的内容以逗号作为分割点，并将每个逗号分隔的字符串内容遍历输出
        string strs = str[0];
        int num=System.Int32.Parse(strs);
       // Debug.Log(num);
        strs = str[1];
        string[] ss = strs.Split(' ');
        double[] omegaD= new double[ss.Length];
        //omegaD = Array.ConvertAll(ss, double.Parse);
        for (int i = 0; i < ss.Length-1; i++)
        {
            omegaD[i] = double.Parse(ss[i]);
            //Debug.Log(omegaD[i]);
        }
        strs = str[2];
        ss = strs.Split(' ');
        double[] c= new double[ss.Length];
        // c = Array.ConvertAll(ss, double.Parse);
        for (int i = 0; i < ss.Length-1; i++)
        {
            c[i] = double.Parse(ss[i]);
          //  Debug.Log(c[i]);
        }
        strs = str[3];
        ss = strs.Split(' ');
        double[] mass= new double[ss.Length];
        for (int i = 0; i < ss.Length - 1; i++)
        {
            mass[i] = double.Parse(ss[i]);
         //   Debug.Log(mass[i]);
        }
        strs = str[4];
        ss = strs.Split(' ');
        double[] frequencies= new double[ss.Length];
        for (int i = 0; i < ss.Length - 1; i++)
        {
            frequencies[i] = double.Parse(ss[i]);
           // Debug.Log(frequencies[i]);
        }

        USound(force, num, omegaD, c, mass, str, frequencies);*/
    }

    // Update is called once per frame
    void FixUpdate()
    {
        if (gen == true)
        {
         //   USound(rho, force);
            gen = false;
        }
        
    }
    Vector3  relativeVelocity;
    void OnCollisionEnter(Collision c)
    {

        //    playImpactSound(c.collider.gameObject, relativeVelocity, contactNormal, contactPoint);
        relativeVelocity = c.relativeVelocity;

        Vector3 force = relativeVelocity * _r.mass / Time.deltaTime;
        int foc = (int)force.magnitude;
        Thread t = new Thread(new ParameterizedThreadStart(B));
        t.Start(foc/10);
        
     
        
    }
    private void B(object obj)
    {
      //  USound(rho, youngs, System.Int32.Parse(obj.ToString()));
        USound(num,mat, System.Int32.Parse(obj.ToString()));
        // Debug.Log( obj.ToString());youngs,
    }
}
