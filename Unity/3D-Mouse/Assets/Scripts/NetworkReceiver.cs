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
    public GameObject buttonDisplay;
    public float scaleFactor = 40;

    private string stoff;

    float ax = 0;
    float ay = 0;
    float az = 0;
    float gx = 0;
    float gy = 0;
    float gz = 0;

    bool button;

    public bool receivedIP = false;
    public IPAddress mouseIP = IPAddress.Any;
    public static int PORT = 8888;


    void Start()
    {
        m_Thread = new Thread(new ThreadStart(Run));
        m_Thread.Start();
    }

    public void Run()
    {
        m_Client = new UdpClient(PORT);

        while (true)
        { 
            if (receivedIP)
            {
                IPEndPoint endpoint = new IPEndPoint(IPAddress.Any, 0);

                byte[] receiveBytes = m_Client.Receive(ref endpoint);
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
                    int temp = 0;

                    int.TryParse(split[9], out temp);
                    if (temp == 0)
                        button = false;
                    else
                        button = true;
                }

            }
            else
            {
                IPEndPoint endpoint = new IPEndPoint(IPAddress.Any, 0);

                byte[] receiveBytes = m_Client.Receive(ref endpoint);
                //if(receiveBytes[0] == 0x42)
                {
                    receivedIP = true;
                    mouseIP = endpoint.Address;
                    Debug.Log("Received IP: " + endpoint.Address.ToString());
                }
            }
        }

    }

    void FixedUpdate()
    {
        transform.Rotate(new Vector3(gx / scaleFactor, gy / scaleFactor, gz / scaleFactor));
    }

    void Update()
    {
        text.text = stoff;
        if (Input.GetKeyDown(KeyCode.Space) || Input.touchCount > 0)
        {
            transform.localRotation = Quaternion.identity;
        }
        Debug.Log(button);
        buttonDisplay.SetActive(button);        
    }

    void OnApplicationQuit()
    {
        m_Client.Close();
        m_Thread.Abort();
    }
}
