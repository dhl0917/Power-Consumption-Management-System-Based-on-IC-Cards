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
    public partial class adminlogin : Form
    {
        public adminlogin()
        {
            InitializeComponent();
        }
        public static string tString;   //携带给admin窗口
        private void button1_Click(object sender, EventArgs e)
        {
            //生成链接数据库字符串
            string conStr = "provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=" + "myNewIC.mdb";
            //创建数据库连接
            OleDbConnection conn = new OleDbConnection(conStr);
            //打开数据库连接
            conn.Open();
            OleDbDataReader myReader = null;
            //sql语句        
            string sql = "select * from myTable where cstr(卡号)='" + textBox1.Text+"'";
            //命令
            OleDbCommand cmd = new OleDbCommand(sql, conn);
            myReader = cmd.ExecuteReader();
            if (myReader.Read())
            {
                if (!myReader.GetBoolean(1))    //权限
                {
                    MessageBox.Show("此用户没有管理员权限", "Warning");
                }
                else {
                    if (myReader.GetString(5) == textBox2.Text) //检查密码
                    {
                        tString = textBox1.Text;
                        myTab form = new myTab();
                        form.ShowDialog();
                        this.Hide();
                    }
                    else
                    {
                        MessageBox.Show("密码错误！", "Warning");
                    }
                }              
            }
            else
            {
                MessageBox.Show("用户名错误！","Warning");
            }
        }

        private void linkLabel1_MouseClick(object sender, MouseEventArgs e)
        {
            login form = new login();
            form.Show();
            this.Hide();
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Warning form = new Warning();
            form.Show();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
          
        }
    }
}
