using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;//接口
using System.Data.OleDb;    //数据库

namespace test
{
    public partial class user : Form
    {
        public user()
        {
            InitializeComponent();
            textBox1.Text = login.tString;  //从login窗口得到卡号

            myport = new SerialPort();      //初始化串口
            myport.PortName = "com3";
            myport.BaudRate = 9600;
            myport.StopBits = StopBits.One;
            myport.DataBits = 8;
            myport.Parity = Parity.None;

        }
        int flag = 0;
        decimal money;
        SerialPort myport = null;
        private void user_MouseEnter(object sender, EventArgs e)
        {
            if (flag == 0)
            {
                
                string ss = textBox1.Text;
                string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
                //建立数据库引擎连接
                OleDbConnection conn = new OleDbConnection(conStr);
                //建立适配器，通过SQL语句去搜索数据库
                OleDbDataAdapter mydata = new OleDbDataAdapter("select 卡号,户主姓名,挂失位,余额 from myTable where cstr(卡号)='" + ss + "'", conn);
                OleDbDataAdapter mydata2 = new OleDbDataAdapter("select 一季度,二季度,三季度,四季度 from myTable where cstr(卡号)='" + ss + "'", conn);
                //建立数据集
                DataSet datas = new DataSet();
                DataSet datas2 = new DataSet();
                //用FILL的方式将适配器已经连接好的数据表填充到数据集MYDS这张表
                mydata.Fill(datas, 0, 1, "myTable");        //填充表格0开始填1行
                mydata2.Fill(datas2, 0, 1, "myTable");
                //用显示控件来显示表
                tab.DataSource = datas.Tables[0];   //DataGridView显示
                tab2.DataSource = datas2.Tables[0];

                //提取户主姓名
                conn.Open();   
                OleDbDataReader myReader = null;    
                //sql语句        
                string sql = "select * from myTable where cstr(卡号)='" + ss + "'";
                //命令
                OleDbCommand cmd = new OleDbCommand(sql, conn);
                myReader = cmd.ExecuteReader();
                if (myReader.Read())
                {

                    string name = myReader.GetString(2);
                    label1.Text = "尊敬的" + name + ",\n欢迎您来到用户用电信息查询系统！";
                }
                else
                {
                    MessageBox.Show("未查询到该用户","Warning");
                }

                //提取季度用电总和
                sql = "select sum(一季度+二季度+三季度+四季度) from myTable where cstr(卡号)='" + ss + "'";
                cmd = new OleDbCommand(sql, conn);
                string sum = Convert.ToString(cmd.ExecuteScalar());
                string avg_sea = Convert.ToString(Convert.ToDecimal(Convert.ToDecimal(Convert.ToString(cmd.ExecuteScalar())) / 4));

                sql = "select avg(一季度) from myTable where cstr(权限位)='0'";
                cmd = new OleDbCommand(sql, conn);
                decimal sea1 =Convert.ToDecimal( Convert.ToString(cmd.ExecuteScalar()));
                sql = "select avg(二季度) from myTable where cstr(权限位)='0'";
                cmd = new OleDbCommand(sql, conn);
                decimal sea2 = Convert.ToDecimal(Convert.ToString(cmd.ExecuteScalar()));
                sql = "select avg(三季度) from myTable where cstr(权限位)='0'";
                cmd = new OleDbCommand(sql, conn);
                decimal sea3 = Convert.ToDecimal(Convert.ToString(cmd.ExecuteScalar()));
                sql = "select avg(四季度) from myTable where cstr(权限位)='0'";
                cmd = new OleDbCommand(sql, conn);
                decimal sea4 = Convert.ToDecimal(Convert.ToString(cmd.ExecuteScalar()));


                string avg_all= Convert.ToString(sea1+sea2+sea3+sea4);  //所有用户平均值

                string compare = "";
                if ( Convert.ToDecimal(sum)<= Convert.ToDecimal(avg_all) )
                {
                    compare = "低";
                }
                else {
                    compare = "高";
                }

                label4.Text = "四个季度电费总和：  " + sum + "\t元" +"("+compare+"于全体用户平均水平"+avg_all+"元)";    //label显示
                label5.Text= "四个季度电费平均值：" + avg_sea + "\t元";

                //提取作图数据
                sql = "select * from myTable where cstr(卡号)='" + ss + "'";
                cmd = new OleDbCommand(sql, conn);
                myReader = cmd.ExecuteReader();
                decimal[] data = new decimal[4];
                
                if (myReader.Read())
                {
                    data[0] = myReader.GetDecimal(6);
                    data[1] = myReader.GetDecimal(7);
                    data[2] = myReader.GetDecimal(8);
                    data[3] = myReader.GetDecimal(9);
                    money = myReader.GetDecimal(4);
                }
                else {
                    MessageBox.Show("未查询到该用户", "Warning");
                }
                    conn.Close();

                List<string> xData = new List<string>() { "一季度", "二季度", "三季度", "四季度" };
                List<decimal> yData = new List<decimal>() {data[0],data[1],data[2],data[3] };
                List<decimal> zData = new List<decimal>() { sea1, sea2, sea3, sea4 };
                chart1.Series[0].Points.DataBindXY(xData, yData);
                chart1.Series[1].Points.DataBindXY(xData, zData);
            }


        }

        private void button1_Click(object sender, EventArgs e)
        {
            
            
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {

            login form = new login();
            form.Show();
            this.Hide();
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
           
        }

        private void linkLabel3_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
           
        }

        private void linkLabel2_LinkClicked_1(object sender, LinkLabelLinkClickedEventArgs e)
        {
            pictureBox1.Visible = true;
            pictureBox1.ImageLocation = textBox1.Text + ".jpg";
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            pictureBox1.Visible = false;
        }
    }
   
}
