﻿using System.Collections;
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

    private string stoff;

    float ax = 0;
    float ay = 0;
    float az = 0;
    float gx = 0;
    float gy = 0;
    float gz = 0;


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
                
                stoff = data;

                string[] split = data.Split('\n');
                
                float.TryParse(split[1], out ax);
                float.TryParse(split[2], out ay);
                float.TryParse(split[3], out az);
                float.TryParse(split[5], out gx);
                float.TryParse(split[6], out gy);
                float.TryParse(split[7], out gz);
                

            }
        }

    }

    void FixedUpdate()
    {
        float div = 40;
        transform.Rotate(new Vector3(gx/div, gy/div, gz/div));
    }
	
	void Update () {
        text.text = stoff;
        if(Input.GetKeyDown("r"))
        {
            transform.rotation.Set(0, 0, 0, 0);
        }
    }

    void OnApplicationQuit()
    {
        m_Client.Close();
        m_Thread.Abort();
    }
}
