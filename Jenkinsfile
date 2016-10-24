#!groovy

parallel (


"nyu" : {node ('gin')
                  {
                    deleteDir()
                    checkout scm
                    stage ('build_gin')
                    {
                      sh "./build.sh $WORKSPACE $NODE_NAME"
                    }

                    stage ('run_gin')
                    {
		      parallel (
                      "1" : {sh "cd openfpm_numerics && ./run.sh $WORKSPACE $NODE_NAME 1"},
                      "2" : {sh "cd openfpm_numerics && ./run.sh $WORKSPACE $NODE_NAME 2"},
                      "3" : {sh "cd openfpm_numerics && ./run.sh $WORKSPACE $NODE_NAME 3"},
                      "4" : {sh "cd openfpm_numerics && ./run.sh $WORKSPACE $NODE_NAME 4"}
                      )
                    }
                  }
                 },




"sb15" : {node ('sbalzarini-mac-15')
                  {
                    deleteDir()
                    env.PATH = "/usr/local/bin:${env.PATH}"
                    checkout scm
                    stage ('build_sb15')
                    {
                      sh "./build.sh $WORKSPACE $NODE_NAME NO"
                    }

                    stage ('run_sb15')
                    {
                      parallel (
                      "1" : {sh "cd openfpm_vcluster && ./run.sh $WORKSPACE $NODE_NAME 1"},
                      "2" : {sh "cd openfpm_vcluster && ./run.sh $WORKSPACE $NODE_NAME 2"},
                      "3" : {sh "cd openfpm_vcluster && ./run.sh $WORKSPACE $NODE_NAME 3"},
                      "4" : {sh "cd openfpm_vcluster && ./run.sh $WORKSPACE $NODE_NAME 4"}
                      )
                    }
                  }
                 }

)

