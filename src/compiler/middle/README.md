# Middle Earth

Once the parsed trees are completed they are fed to the semantic analyzer. 
The SA is where dreams go to die. Once an instruction is deemed valid from the SA
it will call into generation to have some IR code generated. 

The SA is what calls into the generator as this ensures all information required to 
generate an instruction is as short lived as possible. There is no point in keeping
meta data around and re-iterating things.
