package com.example.bluetoothcontrol;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import yuku.ambilwarna.AmbilWarnaDialog;

public class MainActivity extends AppCompatActivity implements
        CompoundButton.OnCheckedChangeListener,
        AdapterView.OnItemClickListener,
        View.OnClickListener, AdapterView.OnItemSelectedListener, SeekBar.OnSeekBarChangeListener {

    private static final String TAG=MainActivity.class.getSimpleName();
    private static final int REQ_ENABLE_BT = 10;
    private static final int BT_BOUNDED = 21;
    private static final int BT_SEARCH = 22;
    private static final int REQUEST_CODE_LOC = 1;
    private static boolean spinnerFirstRun = false;
    private static boolean appFirstRun = true;
    private static int seekBarChangeValue = 0;
    private static int currentEffect = 0;
    private static int runningEffect = -1;
    private static int currentExtraValue = -1;
    private static int maxValue;
    private static boolean extraFlag = false;
    private int currentColor;
    private String currentColorString;

    private FrameLayout frameMessage;
    private LinearLayout frameControl;
    private RelativeLayout frameControls;
    private Button btnDisconnect;
    private Spinner spinnerOptions;
    private TextView extraOption;
    private EditText extraValue;
    private SeekBar seekBarBrightness;
    private SeekBar seekBarSpeed;
    private Button btnStartEffect;
    private Button btnStopEffect;
    private Button btnChooseColor;
    private Button btnSendText;
    private EditText editText;

    private Switch switchOnOffBt;
    private Button btnSearch;
    private ProgressBar pbProgressBar;
    private ListView listBtDevices;

    private BluetoothAdapter bluetoothAdapter;
    private BtListAdapter listAdapter;
    private ArrayList<BluetoothDevice> bluetoothDevices;

    private ConnectThread connectThread;
    private ConnectedThread connectedThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        frameMessage = findViewById(R.id.frame_message);
        frameControl = findViewById(R.id.frame_ontrol);
        frameControls = findViewById(R.id.frame_effects_control);

        switchOnOffBt   = findViewById(R.id.switch_bt);
        btnSearch       = findViewById(R.id.bt_search_button);
        pbProgressBar   = findViewById(R.id.pb_progress);
        listBtDevices   = findViewById(R.id.lv_device);

        btnDisconnect = findViewById(R.id.btn_disconnect);
        spinnerOptions = findViewById(R.id.options_spinner);
        extraOption = findViewById(R.id.extra_option);
        extraValue = findViewById(R.id.extra_value);
        seekBarBrightness = findViewById(R.id.seek_Bar_Brightness);
        seekBarSpeed = findViewById(R.id.seek_Bar_Speed);
        btnStartEffect = findViewById(R.id.btn_effectStart);
        btnStopEffect = findViewById(R.id.btn_effectStop);
        btnChooseColor = findViewById(R.id.btn_colorSelect);
        btnSendText = findViewById(R.id.btn_send_text);
        editText = findViewById(R.id.text_input);

        //listeners para elemntos de la interfaz
        switchOnOffBt.setOnCheckedChangeListener(this);
        btnSearch.setOnClickListener(this);
        listBtDevices.setOnItemClickListener(this);
        btnDisconnect.setOnClickListener(this);
        spinnerOptions.setOnItemSelectedListener(this);
        seekBarBrightness.setOnSeekBarChangeListener(this);
        seekBarSpeed.setOnSeekBarChangeListener(this);
        btnStartEffect.setOnClickListener(this);
        btnStopEffect.setOnClickListener(this);
        btnSendText.setOnClickListener(this);
        btnChooseColor.setOnClickListener(this);

        //inicializo bluetooth adapter
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bluetoothDevices = new ArrayList<>();

        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        filter.addAction(BluetoothDevice.ACTION_FOUND);
        registerReceiver(receiver,filter);

        if (bluetoothAdapter==null){
            //si el bluetooth no esta soportado se muestra un mensaje y se cierra la app
            showToastMessage("Bluetooth no soportado");
            Log.d(TAG, "onCreate: "+getString(R.string.bt_not_supported));
            finish();
        }

        if (bluetoothAdapter.isEnabled()){
            showFrameControl();
            switchOnOffBt.setVisibility(View.VISIBLE);
            switchOnOffBt.setChecked(true);
            setListAdapter(BT_BOUNDED);
        } else {
            showFrameMessage();
            switchOnOffBt.setVisibility(View.VISIBLE);
            switchOnOffBt.setChecked(false);
        }
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
        unregisterReceiver(receiver);

        if (connectThread != null){
            connectThread.cancel();
        }

        if (connectedThread != null){
            connectedThread.cancel();
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    public void onClick(View v) {
        if (v.equals(btnSearch)) {
            enableSearch();
        } else if (v.equals(btnDisconnect)) {
            btnDisconnectRoutine();
        } else if (v.equals(btnStartEffect)){
            startEffect();
        } else if (v.equals(btnStopEffect)){
            stopEffect();
        } else if (v.equals(btnSendText)){
            sendTexRoutine();
        } else if (v.equals(btnChooseColor)){
            openDialog();
        }
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        if (parent.equals(listBtDevices)){
            BluetoothDevice device = bluetoothDevices.get(position);
            if (device != null){
                connectThread = new ConnectThread(device);
                connectThread.start(); //conexion con el dispositivo
            }
        }
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        if (buttonView.equals(switchOnOffBt)){
            enableBT(isChecked);

            if(!isChecked){
                showFrameMessage();
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQ_ENABLE_BT) {
            if (resultCode == RESULT_OK && bluetoothAdapter.isEnabled()) { //se comprueba que el bluetooth esta se ha encendido
                showFrameControl();
                setListAdapter(BT_BOUNDED);
            } else if (resultCode == RESULT_CANCELED) {
                enableBT(true); //se le haran peticiones al usuario hasta que de permiso de encender el bluetooth
            }
        }
    }

    private void showFrameMessage(){
        switchOnOffBt.setVisibility(View.VISIBLE);
        frameMessage.setVisibility(View.VISIBLE);
        frameControl.setVisibility(View.GONE);
        frameControls.setVisibility(View.GONE);
    }

    private void showFrameControl(){
        switchOnOffBt.setVisibility(View.VISIBLE);
        frameControl.setVisibility(View.VISIBLE);
        frameMessage.setVisibility(View.GONE);
        frameControls.setVisibility(View.GONE);

    }

    private void showFrameEffectsControls(){ //marco con los controles de los efectos visuales
        createSpinnerAdapter();
        frameControls.setVisibility(View.VISIBLE);
        frameMessage.setVisibility(View.GONE);
        frameControl.setVisibility(View.GONE);
        switchOnOffBt.setVisibility(View.GONE);
        if (appFirstRun) appFirstRun = false;
    }

    private void createSpinnerAdapter(){
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.effects_options, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinnerOptions.setAdapter(adapter);
    }

    private void showFrameTextInput(){
        editText.setVisibility(View.VISIBLE);
        btnSendText.setVisibility(View.VISIBLE);
    }

    private void hideFrameTextInput(){
        editText.setVisibility(View.GONE);
        btnSendText.setVisibility(View.GONE);
    }

    private void showColorButton(){
        btnChooseColor.setVisibility(View.VISIBLE);
    }

    private void hideColorButton(){
        btnChooseColor.setVisibility(View.GONE);
    }

    private void startEffect(){
        if(currentEffect != 10){
            getExtraOption();
            if (runningEffect != currentEffect){
                runningEffect = currentEffect;
                showToastMessage("Efecto seleccionado: "+runningEffect);
                sendData("$0 "+currentEffect+";");
            }

            if (extraFlag){
                extraFlag = false;
                if (currentExtraValue <= maxValue){
                    sendData("$5 "+currentExtraValue+";");
                } else {
                    showToastMessage("Valor maximo superado");
                    clearExtraOption();
                }
            }
        }

        sendData("$6 1;");
    }

    private void stopEffect(){
        sendData("$6 0;");
    }

    private void showExtraOptions(String s, String hint, int max){
        maxValue = max;
        extraOption.setText(s);
        extraValue.setHint(hint);
        extraOption.setVisibility(View.VISIBLE);
        extraValue.setVisibility(View.VISIBLE);
    }

    private void hideExtraOptions(){
        extraOption.setVisibility(View.GONE);
        extraValue.setVisibility(View.GONE);
    }

    private void getExtraOption(){
        if (!extraValue.getText().toString().isEmpty()){
            currentExtraValue = Integer.parseInt(extraValue.getText().toString());
            extraFlag = true;
        } else {
            currentExtraValue = -1;
        }
    }

    private void clearExtraOption(){
        extraValue.getText().clear();
        currentExtraValue = -1;
    }

    private void showToastMessage(String s){
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
    }

    private void openDialog(){
        AmbilWarnaDialog dialog = new AmbilWarnaDialog(this, currentColor, new AmbilWarnaDialog.OnAmbilWarnaListener() {
            @Override
            public void onOk(AmbilWarnaDialog dialog, int color) {
                currentColor = color;
                currentColorString = String.format("%06X", (0xFFFFFF & color));
                sendData("$4 "+ currentColorString +";");
            }

            @Override
            public void onCancel(AmbilWarnaDialog dialog) {
            }
        });
        dialog.show();
    }

    private void btnDisconnectRoutine(){
        if (connectThread != null){
            connectThread.cancel();
        }

        if (connectedThread != null){
            connectedThread.cancel();
        }
        spinnerFirstRun = false;
        currentEffect = 0;
        hideColorButton();
        hideExtraOptions();
        showFrameControl();
    }

    private void sendTexRoutine(){
        String s = editText.getText().toString();
        if (s.isEmpty() || s.startsWith(" ")){
            showToastMessage("No se permite enviar campo vacio");
        } else {
            sendData("$3 "+s);
        }
        editText.getText().clear();
    }

    private void sendData(String s){ // metodo para enviar datos por bluetooth
        connectedThread.write(s);
    }

    //metodo para encender bluetooth
    private void enableBT(boolean b){
        if (b){
            Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(intent,REQ_ENABLE_BT);
        } else {
            bluetoothAdapter.disable();
        }
    }

    private void setListAdapter(int type){
        bluetoothDevices.clear();
        int iconType = R.drawable.ic_bluetooth_bounded_device;

        switch (type) {
            case BT_BOUNDED:
                bluetoothDevices = getBoundedBtDevices();
                iconType = R.drawable.ic_bluetooth_bounded_device;
                break;
            case BT_SEARCH:
                iconType = R.drawable.ic_bluetooth_search_device;
                break;
        }
        listAdapter = new BtListAdapter(this, bluetoothDevices, iconType);
        listBtDevices.setAdapter(listAdapter);
    }

    private ArrayList<BluetoothDevice> getBoundedBtDevices() {
        Set<BluetoothDevice> deviceSet = bluetoothAdapter.getBondedDevices();
        ArrayList<BluetoothDevice> tmpArrayList = new ArrayList<>();
        if (deviceSet.size() > 0) {
            tmpArrayList.addAll(deviceSet);
        }

        return tmpArrayList;
    }

    //metodo que habilita la busqueda de dispositivos
    @RequiresApi(api = Build.VERSION_CODES.M)
    private void enableSearch() {
        if (bluetoothAdapter.isDiscovering()) {
            bluetoothAdapter.cancelDiscovery();
        } else {
            accessLocationPermission();
            bluetoothAdapter.startDiscovery();
        }
    }

    //broadcast receiver
    private BroadcastReceiver receiver;

    {
        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                final String action = intent.getAction();
                assert action != null;
                switch (action) {
                    case BluetoothAdapter.ACTION_DISCOVERY_STARTED:
                        btnSearch.setText(R.string.stop_search);
                        pbProgressBar.setVisibility(View.VISIBLE);
                        setListAdapter(BT_SEARCH);
                        break;
                    case BluetoothAdapter.ACTION_DISCOVERY_FINISHED:
                        btnSearch.setText(R.string.start_search);
                        pbProgressBar.setVisibility(View.GONE);
                        break;
                    case BluetoothDevice.ACTION_FOUND:
                        BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                        if (device != null) {
                            bluetoothDevices.add(device);
                            listAdapter.notifyDataSetChanged();
                        }
                        break;
                }
            }
        };
    }

    //permisos especiales para versiones de android superiores a 6.0

    @RequiresApi(api = Build.VERSION_CODES.M)
    private void accessLocationPermission() {
        int accessCoarseLocation = 0;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            accessCoarseLocation = this.checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION);
        }
        int accessFineLocation   = 0;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            accessFineLocation = this.checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION);
        }

        List<String> listRequestPermission = new ArrayList<>();

        if (accessCoarseLocation != PackageManager.PERMISSION_GRANTED) {
            listRequestPermission.add(Manifest.permission.ACCESS_COARSE_LOCATION);
        }
        if (accessFineLocation != PackageManager.PERMISSION_GRANTED) {
            listRequestPermission.add(Manifest.permission.ACCESS_FINE_LOCATION);
        }

        if (!listRequestPermission.isEmpty()) {
            String[] strRequestPermission = listRequestPermission.toArray(new String[0]);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                this.requestPermissions(strRequestPermission, REQUEST_CODE_LOC);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == REQUEST_CODE_LOC) {
            if (grantResults.length > 0) {
                // Check if request is granted or not
                for (int gr : grantResults)
                    if (gr != PackageManager.PERMISSION_GRANTED) return;
                //aqui podria or el codigo para hacer el dispositivo visible, en este caso no hace falta
            }
        }
    }

    //spinner, seleciionador de modos
    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

        if(spinnerFirstRun){
            currentEffect = position;
            hideColorButton();
            hideFrameTextInput();
            clearExtraOption();
            if (currentEffect == 1){
                showExtraOptions("Intensidad de nieve","1-10",10);
                showColorButton();
            } else if (currentEffect == 9){
                showExtraOptions("Densidad de estrellas","10-50",50);
            } else if (currentEffect == 10){ // efecto texto
                hideExtraOptions();
                showColorButton();
                showFrameTextInput();
            } else {
                hideExtraOptions();
                hideFrameTextInput();
            }
        } else {
            spinnerFirstRun = true;
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {

    }

    //seekBar, barras de control de brillo y velocidad
    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        seekBarChangeValue = progress;
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {}

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        String s = String.valueOf(seekBarChangeValue);
        if (seekBar.equals(seekBarBrightness)){
            sendData("$1 "+s+";");
        } else if (seekBar.equals(seekBarSpeed)){
            sendData("$2 "+s+";");
        }
    }

    //clase para realizar la conexion con bt dispositivo
    private class ConnectThread extends Thread {

        private BluetoothSocket bluetoothSocket = null;
        private boolean success = false;

        ConnectThread(BluetoothDevice device) {
            try {
                @SuppressWarnings("JavaReflectionMemberAccess") Method method = device.getClass().getMethod("createRfcommSocket", int.class);
                bluetoothSocket = (BluetoothSocket) method.invoke(device, 1);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            try {
                bluetoothSocket.connect();
                success = true;
            } catch (IOException e) {
                e.printStackTrace();

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        showToastMessage("No se puede establecer conexion");
                    }
                });

                cancel();
            }

            if (success) {
                connectedThread = new ConnectedThread(bluetoothSocket);
                connectedThread.start();

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        showFrameEffectsControls();
                    }
                });
            }
        }

        /*public boolean isConnect() {
            return bluetoothSocket.isConnected();
        }*/

        void cancel() {
            try {
                bluetoothSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private class ConnectedThread  extends  Thread {

        private final InputStream inputStream;
        private final OutputStream outputStream;

        ConnectedThread(BluetoothSocket bluetoothSocket) {
            InputStream inputStream = null;
            OutputStream outputStream = null;

            try {
                inputStream = bluetoothSocket.getInputStream();
                outputStream = bluetoothSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }

            this.inputStream = inputStream;
            this.outputStream = outputStream;
        }

        @Override
        public void run() {

        }

        void write(String command) {
            byte[] bytes = command.getBytes();
            if (outputStream != null) {
                try {
                    outputStream.write(bytes);
                    outputStream.flush();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        void cancel() {
            try {
                inputStream.close();
                outputStream.close();
            }catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
