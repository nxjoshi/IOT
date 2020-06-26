const oracledb = require('oracledb');
const oracleConfig = require('./database.conf');

async function initialize() {
    const pool = await oracledb.createPool(oracleConfig.cvgData);
}

module.exports.initialize = initialize;

async function close() {
    await oracledb.getPool().close();
}

module.exports.close = close;

function insertIOTData(data){
    //data json with humidity and temprature
    return new Promise(async function(resolve,reject){
        let connection;
       
        try{

            opts = {
                autoCommit: true,
                // batchErrors: true,  // continue processing even if there are data errors
                bindDefs: {
                  
                    HUMIDITY: { type: oracledb.NUMBER },
                    TEMPRATURE:{ type: oracledb.NUMBER }
                   
                }
              };
              connection = await oracledb.getConnection();
              let statement =`INSERT INTO IOT(HUMIDITY,TEMPRATURE) VALUES (:HUMIDITY,:TEMPRATURE)`;
              await connection.execute(statement,data,opts); 
              resolve(true);


        }catch(e){
            console.log("error while inserting");
            reject(e);
        }finally{
            if(connection){
                try{
                    await connection.close();
                }catch(e){
                    reject(e);
                }
                
            }

        }

    });

}

module.exports.insertIOTData = insertIOTData;
