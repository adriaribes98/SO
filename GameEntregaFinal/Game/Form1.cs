using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Game
{
    public partial class Form1 : Form
    {
        Socket servidor;
        Thread atender;
        string mensajetexto;
        string numsocket;
        string username;
        string password;
        string Usuarioinvitado;
        string rname, rpassword;
        delegate void DelegadoParaEscribir(string text);
        delegate void DelegadoParaActualizarLista(string[] nombres, int num);
        delegate void DelegadoParaGroupBox();
        List<string> conectados = new List<string>();
        List<string> invitados = new List<string>();
        string chatid;

        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            
        }

        public void SetName(string username1)
        {
            rname = username1;
        }
        public void SetPassword(string password1)
        {
            rpassword = password1;
        }

        private void AtenderServidor()
        {
            while (true)
            {
                byte[] msg2 = new byte[500];
                servidor.Receive(msg2);
                string[] trozos = Encoding.ASCII.GetString(msg2).Split('/');
                int codigo = Convert.ToInt32(trozos[0]);

                switch (codigo)
                {
                    case 2:
                        MessageBox.Show("Sesión iniciada correctamente");
                        break;

                    case 8:
                        String[] trozos1 = trozos[1].Split(',');
                        int n = Convert.ToInt32(trozos1[0]);
                        dataGridView1.RowCount = n;
                        dataGridView1.ColumnCount = 2;
                        int j = 0;
                        for (int i = 0; i < n; i++)
                        {
                            j++;
                            dataGridView1[0, i].Value = trozos1[j];
                            j++;
                            dataGridView1[1, i].Value = trozos1[j];
                        }
                        break;
                    case 9:
                        chatid = trozos[1];
                        textBox5.Text = (trozos[2] + " : Te ha invitado a jugar una partida");
                        //textBox5.Text = (trozos[3] + " : " + trozos[2] + " te ha invitado a jugar una partida");
                        //MessageBox.Show("Usuario invitado correctamente");
                        break;
                    case 10:
                        textBox7.Text = trozos[1];
                        break;
                    case 11:
                        if (trozos[2] == textBox2.Text)
                        {
                            listBox1.Items.Add("Yo" + ":" + trozos[1]);
                        }
                        else
                        {
                            listBox1.Items.Add(trozos[2] + ":" + trozos[1]);
                        }
                        break;
                    case 12:
                        MessageBox.Show("El usuario que ha invitado tiene socket" + trozos[1]);
                        MessageBox.Show("El usuario que ha invitado tiene socket" + trozos[2]);
                        MessageBox.Show("El chat esta listo");
                        break;
                    case 24:
                        textBox5.Text = trozos[1];
                        break;






                }




            }

        }



        private void button1_Click(object sender, EventArgs e)//BOTON DE LAS CONSULTAS
        {
            try
            {
                if (checkBox1.Checked)
                {
                    string mensaje = "4/" + textBox1.Text;
                    //enviar al servidor
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    servidor.Send(msg);
                    //respuesta del servidor
                    byte[] msg2 = new byte[500];
                    servidor.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show(mensaje);
                }

                if (checkBox2.Checked)
                {
                    string mensaje = "5/" + textBox1.Text;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    servidor.Send(msg);

                    byte[] msg2 = new byte[500];
                    servidor.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show(mensaje);
                }

                if (checkBox3.Checked)
                {
                    string mensaje = "6/" + textBox1.Text;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    servidor.Send(msg);

                    byte[] msg2 = new byte[500];
                    servidor.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show(mensaje);
                }

            }
            catch
            {
                MessageBox.Show("ERROR");
            }
        }


        private void button2_Click(object sender, EventArgs e)//BOTON CONECTAR
        {
            
            try
            {
                
                //datos del servidor al que queremos conectarnos
                IPAddress direccion = IPAddress.Parse("192.168.56.102");
                IPEndPoint ipep = new IPEndPoint(direccion, 9070);
                //Creamos el socket 
                servidor = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                try
                {
                    servidor.Connect(ipep);//Intentamos conectar el socket
                    this.BackColor = Color.Green;
                    MessageBox.Show("Conectado");

                }
                catch (SocketException)
                {
                    //Si hay excepcion imprimimos error y salimos del programa con return 
                    MessageBox.Show("No he podido conectar con el servidor");
                    return;
                }
                ThreadStart ts = delegate { AtenderServidor(); };
                atender = new Thread(ts);
                atender.Start();
            }
            catch
            {
                MessageBox.Show("ERROR");
            }
        }

        private void button3_Click(object sender, EventArgs e)//BOTON DESCOENCTAR
        {
            try
            {
                //Mensaje de desconexión
                string mensaje = "0/";

                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);

                // Nos desconectamos
                this.BackColor = Color.Gray;
                MessageBox.Show("Desconectado");
                servidor.Shutdown(SocketShutdown.Both);
                servidor.Close();
            }
            catch
            {
                MessageBox.Show("ERROR");
            }
        }





        private void button5_Click(object sender, EventArgs e)//BOTON INICIAR SESION
        {
            try
            {
                username = textBox2.Text;
                password = textBox3.Text;
                string mensaje = "2/" + username + "/" + password;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);
                byte[] msg2 = new byte[500];
                this.Text = textBox2.Text;
                //servidor.Receive(msg2);
                //mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                //int registrado = Convert.ToInt32(mensaje);
                //if (registrado == 2)
                {
                    //     MessageBox.Show("Sesión iniciada correctamente");
                }
                //else
                {
                    //    MessageBox.Show("Datos incorrectos");
                    //   finalizado = true;
                }
            }
            catch
            {
                //    MessageBox.Show("ERROR");
            }
        }


        private void button6_Click(object sender, EventArgs e)
        {
            try
            {

                string mensaje = "7/";
                //enviamos al servidor
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);
                //recibimos respuesta del servidor
                byte[] msg2 = new byte[512];
                //servidor.Receive(msg2);
                //mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                //mostrar lista de conectados en el datagridview
                String[] trozos = mensaje.Split(',');
                int n = Convert.ToInt32(trozos[0]);
                dataGridView1.RowCount = n;
                dataGridView1.ColumnCount = 2;
                int j = 0;
                for (int i = 0; i < n; i++)
                {
                    j++;
                    dataGridView1[0, i].Value = trozos[j];
                    j++;
                    dataGridView1[1, i].Value = trozos[j];
                }
            }
            catch
            {
                MessageBox.Show("ERROR");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //button10.Enabled = false;
           

        }

        private void button4_Click(object sender, EventArgs e)
        {
            button10.Enabled = true; 
            try
             {
                 Usuarioinvitado = textBox4.Text;
                 string mensaje = "9/" + Usuarioinvitado;
                 byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                 servidor.Send(msg);
                 byte[] msg2 = new byte[500];
             }
             catch
             {
             }
    

        }

        private void button7_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Has aceptado la Partida");
            button10.Enabled = true;
            string mens = "12/" + chatid +"/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mens);
            servidor.Send(msg);
            byte[] msg2 = new byte[500];

        }

        private void button8_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Has rechazado la Partida");
            button10.Enabled = false;

        }

        private void button9_Click(object sender, EventArgs e)
        {
            try
            {
                numsocket = textBox6.Text;
                string mensaje = "10/" + numsocket;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);
                byte[] msg2 = new byte[500];
            }
            catch
            {
            }
        }

        

        private void button10_Click_1(object sender, EventArgs e)
        {
            try
            {
                mensajetexto = textBox8.Text;
                string mensaje = "11/" + mensajetexto + "/" + textBox2.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                servidor.Send(msg);
                byte[] msg2 = new byte[500];
                textBox8.Text = "";
              
            }
            catch
            {
            }

        }

       







    }
}








