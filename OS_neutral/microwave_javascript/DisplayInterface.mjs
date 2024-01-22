// Simple helper to interact with the HTML GUI
export function DisplayInterface() {

  // set time text
  this.setTT = function (value){
    document.getElementById("time").innerText=value;
  }

  this.updateImage = function (image){
    document.getElementById("myImage").src = image;
  }
}
