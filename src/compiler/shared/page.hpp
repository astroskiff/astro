/*
  This class represents a raw piece of source code from disk that we can
  interact with. The idea of pages is helpful when it comes to error reporting
  and getting specific data. At first the page is something that will read in
  the entirity of a file from disk and we can operate with it from memory, but
  if astro gets to a point where bigger programs are being written in it, it may
  be wise to update the page object to be an iterface to the file on disk. That
  way we can just load chunks from disk as required.
*/

#ifndef COMPILER_SOURCE_PAGE_HPP
#define COMPILER_SOURCE_PAGE_HPP

#include <string>
#include <vector>

namespace compiler {
namespace shared {

//! \brief A 'page' of source code
class page_c {
public:
  //! \brief Load a page from file
  //! \param file The file to load in
  //! \returns true iff the file coule be loaded in
  bool load_page(const std::string &file);

  //! \brief Get the size of the page in terms of lines
  std::size_t get_page_size() const { return _data.size(); }

  //! \brief Check if the page contains any data
  //! \returns true iff there was data loaded in
  bool has_data() const { return !_data.empty(); }

  //! \brief Retrieve a line from the page
  //! \param line The line to retrieve from the page
  //! \returns bool, string tuple where bool is true iff the line
  //!          was valid and represented a line of data
  std::tuple<bool, std::string> get_line(std::size_t line);

  //! \brief Get the name of the file loaded in
  std::string get_file() const { return _file; }

private:
  std::string _file;
  std::vector<std::string> _data;
};

} // namespace shared
} // namespace compiler

#endif