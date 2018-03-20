import processing.serial.*;

Serial port; //Porta serial
String str; //String a ser lida
PrintWriter output; //"Ponteiro" para o arquivo de saída


void setup(){
  String portName = "/dev/ttyUSB0"; //Atribui o nome da porta
  port = new Serial(this, portName, 9600); //Cria o objeto Serial
  port.clear(); //Esvazia o buffer
  output = createWriter("saida.txt");
}

void draw(){
  if(port.available() > 0){ //Se a porta está disponivel
    str = port.readStringUntil('\n'); //Le a linha
    if(str != null){
      //print(str);  //Imprime a linha no terminal
      output.print(str); //Imprime a linha no arquivo
    }    
  } 
}

void keyPressed() {
    output.flush();  // Escreve o resto dos dados no arquivo
    output.close();  // Fecha o arquivo
    exit();  // Termina a execução do programa
}