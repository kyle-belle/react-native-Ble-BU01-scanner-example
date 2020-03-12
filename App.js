/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow
 */

import React, {Component} from 'react';
import {
  SafeAreaView,
  StyleSheet,
  ScrollView,
  View,
  Text,
  StatusBar,
  TouchableOpacity,
  NativeEventEmitter,
  FlatList
} from 'react-native';

import {
  Header,
  LearnMoreLinks,
  Colors,
  DebugInstructions,
  ReloadInstructions,
} from 'react-native/Libraries/NewAppScreen';

import BleNative from "./src/Components/Native/ble-native"; 

class App extends Component{

  state = {readers: [], searching: false, connectedToDevice: false, connectedIndex: null, connectedReader: null, scanning: false, scannedTags: []};

  BleEventEmitter;
  eventSubscription;
  BleIniteventSubscription;
  BleDeviceDiscoverEventSubscription;
  BleStartSearchEventSubscription;
  BleStopSearchEventSubscription;

  constructor(){
    super();

    this.BleEventEmitter = new NativeEventEmitter(BleNative);
    this.eventSubscription = this.BleEventEmitter.addListener("Event", (result) => {
      console.log(result);
    });
    this.BleIniteventSubscription = this.BleEventEmitter.addListener("BleServiceInit", (res) => {console.log(res)})

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("StartBleSearch", (res) => {
      console.log(res);
      this.setState({readers: [], searching: true});
    });

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("FinishBleSearch", (res) => {
      console.log(res);
      this.setState({searching: false});
    });

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("StopBleSearch", (res) => {
      console.log(res);
      this.setState({searching: false});
    });

    this.BleDeviceDiscoverEventSubscription = this.BleEventEmitter.addListener("BleDeviceDiscovered", (res) => {
      console.log(res);
      this.state.readers.push(res);
      this.setState({readers: this.state.readers});
    })

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("ConnectionSuccess", (res) => {
      console.log(res);
      this.setState({searching: false, connectedToDevice: true});
    });

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("ConnectionFail", (res) => {
      console.log(res);
      this.setState({searching: false, connectedToDevice: false, connectedIndex: null, connectedReader: null});
    });

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("ConnectionDisconnect", (res) => {
      console.log(res);
      this.setState({searching: false, scanning: false, connectedToDevice: false, connectedIndex: null, connectedReader: null});
    });
    
    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("StartScanning", (res) => {
      console.log(res);
      this.setState({scanning: true});
    });

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("StopScanning", (res) => {
      console.log(res);
      this.setState({scanning: false});
    });

    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("ErrorScanning", (res) => {
      console.log(res);
      console.log(BleNative.ScanCodes[`${res.errorCode}`]);
      // alert(`scanning error: ${res.errorCode}`);
    });
    
    this.BleStartSearchEventSubscription = this.BleEventEmitter.addListener("TagScanned", (res) => {
      console.log(res);
      const index = this.state.scannedTags.findIndex((tag, i) => {
        return tag.epc === res.ScannedTag.epc;
      })
      console.log(`index: ${index}`)
      if(index === -1){
        this.state.scannedTags.push(res.ScannedTag)
      }else{
        this.state.scannedTags[index] = res.ScannedTag;
      }
      if(!this.state.scanning){
        this.setState({scanning: true});
        return;
      }
      this.setState();
    });

    BleNative.BleInit((err, msg) => {
      console.log(msg);
    });


  }

  onPressSearch(){
    if(!this.state.searching){
      BleNative.searchForBleReader((err, res) => {
        console.log(res);
        this.setState({searching: false});
      })
    }
  }

  render(){
    
    return (
      <>
      <StatusBar barStyle="light-content" />
      <SafeAreaView style={{flex: 1, backgroundColor: "rgb(20,40,80)"}}>
        <View style={{flex: 1, backgroundColor: "white"}}>
          <View style={styles.header}><Text style={styles.headerText}>BleReader</Text></View>
          {this.state.searching?<Text>searching...</Text>:null}
          <View style={{flexDirection: "row", justifyContent: "space-evenly"}}>
            <TouchableOpacity style={styles.button} onPress={() => {BleNative.addEvent("Ble", "Native", (error, name, location) => {/* alert(`name: ${name} location: ${location}`)*/}) }}>
              <Text style={styles.buttonText}>BUTTON</Text>
            </TouchableOpacity>
            
            <TouchableOpacity style={styles.button} onPress={this.onPressSearch.bind(this)}>
              <Text style={styles.buttonText}>SEARCH</Text>
            </TouchableOpacity>
          </View>

          <FlatList style={{flex: 1, backgroundColor:"rgb(220,220,255)"}} data={this.state.readers} renderItem={({item, index}) => {
            return (
              <View style={[styles.deviceListItemStyle, {borderLeftColor: (this.state.connectedIndex === index && this.state.connectedToDevice)?(this.state.scanning?"limegreen":"gold"):"white"}]}>
                <TouchableOpacity onLongPress={() => {
                  BleNative.disconnectFromBleReader((err, res) => {
                  console.log(res);
                });
            }} onPress={(this.state.connectedIndex === index && this.state.connectedToDevice)?() => {
              BleNative.StartScanning((err) => {
                if(err){
                  console.log(err);
                }
              });
            }:() => {
                  BleNative.connectToBleReader(item, index, () => {this.setState({connectedToDevice: item, connectedIndex: index})});
                }} style={{width: "100%", height: "100%", justifyContent: "center"}}>
                  <Text>{index+1} {item.readerName}</Text>
                </TouchableOpacity>
              </View>
            )
          }} keyExtractor={(item) => {return item.readerName}} />
              <View style={[styles.tagListItemStyle]}>
                <View style={styles.columnStyle}>
                  <Text>EPC</Text>
                </View>
                <View style={styles.columnStyle}>
                  <Text>RSSI</Text>
                </View>
                <View style={styles.columnStyle}>
                  <Text>COUNT</Text>
                </View>
              </View>
          <FlatList style={{flex: 1}} data={this.state.scannedTags} renderItem={({item, index}) => {
            return (
              <View style={[styles.tagListItemStyle]}>
                <View style={styles.columnStyle}>
                  <Text>{item.epc}</Text>
                </View>
                <View style={styles.columnStyle}>
                  <Text>{item.rssi}</Text>
                </View>
                <View style={styles.columnStyle}>
                  <Text>{item.count}</Text>
                </View>
              </View>
            )
          }} keyExtractor={(item) => {return item.readerName}} />
        </View>
      </SafeAreaView>
      </>
    );
  };
}

const styles = StyleSheet.create({
  scrollView: {
    backgroundColor: Colors.lighter,
  },
  engine: {
    position: 'absolute',
    right: 0,
  },
  body: {
    backgroundColor: Colors.white,
  },
  sectionContainer: {
    marginTop: 32,
    paddingHorizontal: 24,
  },
  sectionTitle: {
    fontSize: 24,
    fontWeight: '600',
    color: Colors.black,
  },
  sectionDescription: {
    marginTop: 8,
    fontSize: 18,
    fontWeight: '400',
    color: Colors.dark,
  },
  highlight: {
    fontWeight: '700',
  },
  footer: {
    color: Colors.dark,
    fontSize: 12,
    fontWeight: '600',
    padding: 4,
    paddingRight: 12,
    textAlign: 'right',
  },
  header: {
    // flex: 1,
    height: 60,
    justifyContent: "center",
    alignItems: "center",
    backgroundColor: "rgb(20,40,80)"
  },
  headerText: {
    color: "white",
    fontSize: 20,
    textAlign: "center"
  },
  deviceListItemStyle: {
    height: 60,
    marginVertical: 5,
    borderLeftWidth: 10,
    borderLeftColor: "white",
    paddingLeft: 10,
  },
  tagListItemStyle: {
    height: 60,
    flexDirection: "row",
    justifyContent: "space-evenly",
    marginVertical: 5,
    paddingHorizontal: 10,
  },
  columnStyle: {
    flex: 1,
    alignItems: "center",
  },
  button: {
    marginVertical: 10,
    backgroundColor: "rgb(20,40,80)",
    paddingHorizontal: 10,
    paddingVertical: 8
  },
  buttonText: {
    color: "white",
    fontSize: 16
  }
});

export default App;
