package com.watersjournal.ottoremote;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;

public class OttoRemote extends BlunoLibrary {
    private Button buttonScan;
    private Button buttonSerialSend;
    private EditText serialSendText;
    private TextView serialReceivedText;
    private ImageButton buttonUp;
    private ImageButton buttonDown;
    private ImageButton buttonLeft;
    private ImageButton buttonRight;
    private ImageButton buttonMusic;
    private long startTime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        startTime = System.currentTimeMillis();

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_otto_remote);

        onCreateProcess();	//onCreate Process by BlunoLibrary

        serialBegin(115200);	//set the Uart Baudrate on BLE chip to 115200

        // Button to move Otto forward
        buttonUp = (ImageButton) findViewById(R.id.upButton);
        buttonUp.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                long diff = System.currentTimeMillis() - startTime;
                // Only send the command if the last one was over a second ago
                // To allow Otto to finish the movement
                if (System.currentTimeMillis() > startTime + 1000) {
                    serialSend("F");
                    startTime = System.currentTimeMillis();
                }
            }
        });

        // Button to move Otto backward
        buttonDown = (ImageButton) findViewById(R.id.downButton);
        buttonDown.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                long diff = System.currentTimeMillis() - startTime;
                // Only send the command if the last one was over a second ago
                // To allow Otto to finish the movement
                if (System.currentTimeMillis() > startTime + 1000) {
                    serialSend("B");
                    startTime = System.currentTimeMillis();
                }
            }
        });

        // Button to move Otto left
        buttonLeft = (ImageButton) findViewById(R.id.leftButton);
        buttonLeft.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                long diff = System.currentTimeMillis() - startTime;
                // Only send the command if the last one was over a second ago
                // To allow Otto to finish the movement
                if (System.currentTimeMillis() > startTime + 1000) {
                    serialSend("L");
                    startTime = System.currentTimeMillis();
                }
            }
        });

        // Button to move Otto right
        buttonRight = (ImageButton) findViewById(R.id.rightButton);
        buttonRight.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                long diff = System.currentTimeMillis() - startTime;
                // Only send the command if the last one was over a second ago
                // To allow Otto to finish the movement
                if (System.currentTimeMillis() > startTime + 1000) {
                    serialSend("R");
                    startTime = System.currentTimeMillis();
                }
            }
        });

        // Button to have Otto play some music
        buttonMusic = (ImageButton) findViewById(R.id.musicButton);
        buttonMusic.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                long diff = System.currentTimeMillis() - startTime;
                if (System.currentTimeMillis() > startTime + 1000) {
                    serialSend("M");
                    startTime = System.currentTimeMillis();
                }
            }
        });


        //initial the button for scanning the BLE device
        buttonScan = (Button) findViewById(R.id.buttonScan);
        buttonScan.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                buttonScanOnClickProcess();		//Alert Dialog for selecting the BLE device
            }
        });
    }

    protected void onResume(){
        super.onResume();
        System.out.println("BlUNOActivity onResume");
        onResumeProcess();					//onResume Process by BlunoLibrary
    }



    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        onActivityResultProcess(requestCode, resultCode, data);		//onActivityResult Process by BlunoLibrary
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onPause() {
        super.onPause();
        onPauseProcess();				//onPause Process by BlunoLibrary
    }

    protected void onStop() {
        super.onStop();
        onStopProcess();				//onStop Process by BlunoLibrary
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        onDestroyProcess();				//onDestroy Process by BlunoLibrary
    }

    //Once connection state changes, this function will be called
    @Override
    public void onConectionStateChange(connectionStateEnum theConnectionState) {
        switch (theConnectionState) {			//Four connection states
            case isConnected:
                buttonScan.setText("Connected");
                break;
            case isConnecting:
                buttonScan.setText("Connecting");
                break;
            case isToScan:
                buttonScan.setText("Scan");
                break;
            case isScanning:
                buttonScan.setText("Scanning");
                break;
            case isDisconnecting:
                buttonScan.setText("isDisconnecting");
                break;
            default:
                break;
        }
    }

    //Once connection data received, this function will be called
    @Override
    public void onSerialReceived(String theString) {

    }

}