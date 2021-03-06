# Get Started With EvoMe
## Install Prelimilaries
In this manual, we assume that you are using a Linux system. Particular to this manual, it is run on a Ubuntu system.

### Git Facts Generator (Please skip this if you already have it installed)
1. Clone this [repo](https://github.com/uxhg/ext-gitfacts)
2. Run the following command
    ```
    cd ext-gitfacts
    cargo build --release
    ```
3. (Optional) You may need to install OpenSSL for cargo build to be successful. For Ubuntu system, run `sudo apt-get install libssl-dev`
4. Verify if the build is successful, run `target/release/gitfacts -h`
`
### Program Facts Generator (Please skip this if you already have it installed)
1. Install JDK 1.8, run `sudo apt-get install openjdk-8-jdk`
2. Verify if the build is successful, run `java -version`
3. (Optional) If you have multiple jdk versions, you can set JDK 1.8 to the default one, run `sudo update-alternatives --set java /usr/lib/jvm/<jdk_name>/bin/java`
4. Install Maven, run `sudo apt install maven`
5. Verify if maven is installed successfully, run `mvn -version` 
6. Set environment variables, run `sudo nano /etc/profile.d/maven.sh`, and input the following
```
export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64
export M2_HOME=/usr/share/maven
export MAVEN_HOME=/usr/share/maven
export PATH=${M2_HOME}/bin:${PATH}
```
7. Activate environment variables, run 
```
sudo chmod +x /etc/profile.d/maven.sh
source /etc/profile.d/maven.sh
```
8. Clone gitslice, run `git clone git@bitbucket.org:liyistc/gitslice.git`
9. Run the following commands
```
cd gitslice
git checkout facts-dl4ql
mvn clean install -DskipTests
```
### Soufflé
```
echo "deb https://dl.bintray.com/souffle-lang/deb-unstable bionic main" | sudo tee -a /etc/apt/sources.list
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 379CE192D401AB61
sudo apt-get update
```
If you are in a different operating system, please refer [here](https://souffle-lang.github.io/install) for more information.

### EvoMe Translator
Please refer to this [guide](https://github.com/LiMengyang990726/EvoMe/tree/master/translator#translator-installation).
## Try out now!
### Command-line interface
1. Install any repository you like and write a corresponding query file
2. Run the following commands
```
cd scripts
python3.7 run.py --repo_path <todo> \
         --gitfacts_path <todo> \
         --output_path <todo> \
         --query_file_path <todo> \
         --evome_path <todo> \
         --cslicer_path <todo>
```
### Web interface
Upcoming!

## Language Specification (Working Version)
```
stmt_list :- stmt (stmt_list)? 

stmt :- "import java" 
        | "define" version_selection_opts 
        | select_stmt 

version_selection_opts :- lower_id string_literal ("," version_selection_opts)? 
                        | lower_id "(" multiple_versions_selection_opts ")" ("," version_selection_opts)? 
                        | lower_id "(" string_literal ".." string_literal ")" ("," version_selection_opts)?

multiple_versions_selection_opts :- string_literal ("," multiple_ versions_selection_opts)? 

select_stmt :- ("from" from_opts)? ("range" range_opts)? ("where" where_opts)? "select" select_opts

select_opts :- expr ("as" lower_id)? (select_opts)?

from_opts :- upper_id lower_id ("," from_opts)?

range_opts :- lower_id "@" lower_id ("," range_opts)

where_opts :- ("exists" | "not exist" | "forall") "(" reason_opts "that" formula ")" ("and" whereopts)?

reason_opts :- lower_id ("," reason_opts)

formula :- "(" formula ")" 
        | formula "and" formula 
        | "not" formula 
        | primary (">=" | ">" | "=" | "!=" | "<" | "<=" | "=") primary 
        | call

primary :- lower_id | string_literal | int_literal | call

call :- lower_id "." lower_id "(" (string_literal)? ")"

expr :- "_" | primary
```
