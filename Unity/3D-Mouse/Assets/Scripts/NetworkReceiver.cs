using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine.UI;

public class NetworkReceiver : MonoBehaviour {

    private UdpClient m_Client;
    private Thread m_Thread;

    public Text text;
    

	void Start () {
        m_Thread = new Thread(new ThreadStart(Run));
        m_Thread.Start();
    }

    public void Run()
    {
        m_Client = new UdpClient(8888);

        while (true)
        {
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Any, 0);

            byte[] receiveBytes = m_Client.Receive(ref endpoint);
            //lock (lockObject)
            {
                string data = Encoding.ASCII.GetString(receiveBytes);
                Debug.Log(data);
                text.text = data;
            }
        }

    }
	
	void Update () {
		
	}

    void OnApplicationQuit()
    {
        m_Client.Close();
        m_Thread.Abort();
    }
}
